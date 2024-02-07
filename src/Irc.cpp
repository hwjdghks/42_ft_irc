#include "Irc.hpp"
#include <iostream> /* test header */
#include <sstream>

Irc::Irc(void) {}

Irc::Irc(const Irc &ref)
{
	*this = ref;
}

Irc::~Irc(void) {}

Irc &Irc::operator=(const Irc &ref)
{
	if (this == &ref)
		return *this;
	/* Edit */
	return *this;
}

void Irc::setPassword(std::string password)
{
	this->password = password;
}

std::size_t Irc::getClientListSize(void)
{
	std::size_t tmp = clients.size();
	return (tmp);
}

int Irc::createClient(int fd, std::string hostname)
{
	if (!(clients.size() < MAX_CLIENT))
		return (FAIL);
	Client new_client;
	new_client.setFd(fd);
	new_client.setHostname(hostname);
	if (password.empty())
		new_client.setRegi(PASS, true);
	this->clients.push_back(new_client);
	return (SUCCESS);
}

t_send_event Irc::executeCommand(int fd, std::string recv_buffer)
{
	std::string commandLine;

	_clearSendEvent();
	// 해당 fd의 클라이언트 찾아오기
	Client *client = searchClient(fd);
	// 메세지 리드버퍼에 저장
	client->addRead_buffer(recv_buffer);
	while (client->isAlive())
	{
		// 명령줄 받아보기
		commandLine = client->getLineOfRead_buffer();
		std::cerr << "!    cmdline    [" << commandLine << "]" << std::endl;
		// 만약 문자열이 공백이라면 loop 종료
		if (commandLine.empty())
			break ;
		std::cerr << "!    execute    [" << commandLine << "]" << std::endl;
		// 리시브 메세지 해석
		IRCMessage recv_msg = parseMessage(commandLine);
		// 명령어에 따라 동작하기
		std::cerr << "    cmd    [" << recv_msg.command << "]" << std::endl;
		if (!client->isRegistered())
			_register_executor(client, recv_msg);
		else
			_command_executor(client, recv_msg);
		std::cerr << "    (SUCCESS)" << std::endl;
	}
	return (send_msg);
}

Client *Irc::searchClient(int fd)
{
	Client *client = NULL;
	// client vector를 순회하여 fd에 해당하는 클라이언트 가져오기
	for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			client = &(*it);
			break ;
		}
	}
	return (client);
}

Client *Irc::searchClient(const std::string &nickname)
{
	Client *client = NULL;
	// client vector를 순회하여 fd에 해당하는 클라이언트 가져오기
	for(std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getNickname() == nickname)
		{
			client = &(*it);
			break ;
		}
	}
	return (client);
}

std::vector<int> Irc::getAllClientFd(void) const
{
	std::vector<int> fds;

	for (std::vector<Client>::const_iterator it = clients.begin(); it != clients.end(); it++)
	{
		fds.push_back(it->getFd());
	}
	return fds;
}

t_send_event Irc::ping(int fd)
{
	_clearSendEvent();
	// 해당 fd의 클라이언트를 찾는다
	Client *client = searchClient(fd);
	// t_send_event에 fd를 넣는다
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(fd);
	_setSendEvent(false, false, false, true, fds);
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	client->addWrite_buffer("PING :" SERVERURL);
	return (send_msg);
}

t_send_event Irc::quit(int fd, const char *msg)
{
	_clearSendEvent();
	// 해당 fd의 클라이언트를 찾는다
	Client *client = searchClient(fd);
	client->setLife(false);
	std::vector<int> fds;
	// 해당 클라이언트가 속한 채널의 유저들을 찾는다
	std::vector<Channel *> &channels = client->getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *curr_ch = *it;
		std::vector<Client *> client_list = curr_ch->getUsers();
		for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
	// 해당 유저들의 fd를 t_send_event에 넣는다
			if ((*cl_it)->getFd() != fd)
			{
				if ((*cl_it)->isAlive())
					fds.push_back((*cl_it)->getFd());
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " QUIT :Quit: " + msg + "\r\n");
			}
	}
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	_setSendEvent(false, false, false, true, fds);
	deleteClient(fd);
	return (send_msg);
}

t_send_event Irc::deleteClient(int fd)
{
	// 해당 fd의 클라이언트를 찾는다
	Client *client = searchClient(fd);
	// 해당 클라이언트가 가입된 채널을 찾아 제거한다
	std::vector<Channel *> channels = client->getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *curr_ch = *it;

		if (curr_ch->isOperator(client->getNickname()))
			curr_ch->delOperator(client->getNickname());
		if (curr_ch->isUser(client->getNickname()))
			curr_ch->delUser(client->getNickname());
	}
	// 클라이언트 벡터에서 해당 클라이언트를 제거한다.
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getFd() == fd)
		{
			clients.erase(it);
			break ;
		}
	}
	return (send_msg);
}

int	Irc::_setSendEvent(bool recv_work, bool recv_time, bool recv_close, bool to_send, std::vector<int> fds)
{
	if (!send_msg.recv_work)
		send_msg.recv_work = recv_work;
	if (!send_msg.recv_time)
		send_msg.recv_time = recv_time;
	if (!send_msg.recv_close)
		send_msg.recv_close = recv_close;
	if (!send_msg.to_send)
		send_msg.to_send = to_send;
	if (send_msg.fds.empty())
		send_msg.fds = fds;
	else if (!fds.empty())
	{
		// send_msg.fds에 fd 추가하기
		std::vector<int>::iterator iter;
		for (iter = fds.begin() ; iter != fds.end() ; iter++)
		{
			std::vector<int>::iterator it;
			for (it = send_msg.fds.begin() ; it != send_msg.fds.end() ; it++)
			{
				if (*it == *iter)
					break;
			}
			if (it == send_msg.fds.end())
				send_msg.fds.push_back(*iter);
		}
	}
	return (SUCCESS);
}

int	Irc::_clearSendEvent()
{
	send_msg.recv_work = false;
	send_msg.recv_time = false;
	send_msg.recv_close = false;
	send_msg.to_send = false;
	send_msg.fds.clear();
	return (SUCCESS);
}

Channel *Irc::searchChannel(const std::string &channelname)
{
	Channel *channel = NULL;
	// Channel vector를 순회하여 fd에 해당하는 클라이언트 가져오기
	for(std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == channelname)
		{
			channel = &(*it);
			break ;
		}
	}
	return (channel);
}

int Irc::_register_executor(Client *client, IRCMessage recv_msg)
{
	if (recv_msg.command == "USER")
		__register_user(client, recv_msg);
	else if (recv_msg.command == "PASS")
		__register_pass(client, recv_msg);
	else if (recv_msg.command == "NICK")
		__register_nick(client, recv_msg);
	else if (__isCommand(recv_msg.command)) // RPL_451_err_notregistered
	{
		std::vector<int> fds;
		fds.push_back(client->getFd());
		_setSendEvent(false, false, false, true, fds);
		client->addWrite_buffer(_451_err_notregistered(SERVERURL, client->getNickname()));
	}
	if (client->isRegistered()) // RPL_001_rpl_welcome
	{
		std::cerr << "[" << password << "]" << std::endl;
		std::cerr << "[" << client->getPassword() << "]" << std::endl;
		if (password.empty() || !password.compare(client->getPassword()))
		{
			std::vector<int> fds;
			fds.push_back(client->getFd());
			_setSendEvent(false, true, false, true, fds);
			client->addWrite_buffer(_001_rpl_welcome(SERVERURL, client->getNickname(), client->makeClientPrefix()));
		}
		else
		{
			_clearSendEvent();
			client->setLife(false);
		}
	}
	return (SUCCESS);
}

/* Irc_regi.cpp에서 구현됨
동작의 정의가 있는 부분이라 일단 주석 처리만 해 둔 상태
추후 동작 확인할 때 사용하고 확인이 완료되어 필요성이 사라진다면 삭제 요망

int Irc::__register_user(Client *client, IRCMessage message)
{
	if (param 부족함)
		// RPL_461_err_needmoreparams
	else if (기존 데이터 존재함)
		// RPL_462_err_alreadyregisterd
	else
		// 동작
	return (SUCCESS);
}

int Irc::__register_pass(Client *client, IRCMessage message)
{
	if (param 없음)
		// RPL_461_err_needmoreparams
	else
		// 동작
	return (SUCCESS);
}

int Irc::__register_nick(Client *client, IRCMessage message)
{
	if (param 없음)
		// RPL_461_err_needmoreparams
	else if (이미 등록된 닉네임)
		// RPL_433_err_nicknameinuse
	else if (사용할 수 없는 문자 포함 ('#', ',', ' ', '@' 등))
		// RPL_432_err_erroneusnickname
	else
		// 동작
	return (SUCCESS);
}
*/

bool Irc::__isCommand(std::string cmd)
{
	const char	*commands[] = {"USER", "PASS", "NICK", "PING", "PONG", "LIST", "WHO", "QUIT", "PRIVMSG", "JOIN", "PART", "TOPIC", "KICK", "INVITE", "MODE"};

	for (int i = 0; i < 15 ; i++)
	{
		if (!cmd.compare(commands[i]))
			return (true);
	}
	return (false);
}

int Irc::_command_executor(Client *client, IRCMessage recv_msg)
{
	if (!client->isAlive())
		return (false);
	if (recv_msg.command == "USER")
		__cmd_user(client, recv_msg);
	else if (recv_msg.command == "PASS")
		__cmd_pass(client, recv_msg);
	else if (recv_msg.command == "NICK")
		__cmd_nick(client, recv_msg);
	else if (recv_msg.command == "PING")
		__cmd_ping(client, recv_msg);
	else if (recv_msg.command == "PONG")
		__cmd_pong(client, recv_msg);
	else if (recv_msg.command == "LIST")
		__cmd_list(client, recv_msg);
	else if (recv_msg.command == "WHO")
		__cmd_who(client, recv_msg);
	else if (recv_msg.command == "QUIT")
		__cmd_quit(client, recv_msg);
	else if (recv_msg.command == "PRIVMSG")
		__cmd_privmsg(client, recv_msg);
	else if (recv_msg.command == "JOIN")
		__cmd_join(client, recv_msg);
	else if (recv_msg.command == "PART")
		__cmd_part(client, recv_msg);
	else if (recv_msg.command == "TOPIC")
		__cmd_topic(client, recv_msg);
	else if (recv_msg.command == "KICK")
		__cmd_kick(client, recv_msg);
	else if (recv_msg.command == "INVITE")
		__cmd_invite(client, recv_msg);
	else if (recv_msg.command == "MODE")
		__cmd_mode(client, recv_msg);
	else
		__not_a_command(client, recv_msg);
	return (SUCCESS);
}

int Irc::__cmd_user(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(false, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else // RPL_462_err_alreadyregisterd
		client->addWrite_buffer(_462_err_alreadyregisterd(SERVERURL, client->getNickname()));
	return (SUCCESS);
}

int Irc::__cmd_pass(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(false, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else // RPL_462_err_alreadyregisterd
		client->addWrite_buffer(_462_err_alreadyregisterd(SERVERURL, client->getNickname()));
	return (SUCCESS);
}

int Irc::__cmd_nick(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(false, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (_isNickInUse(client, message.parameters[0])) // RPL_433_err_nicknameinuse
		client->addWrite_buffer(_433_err_nicknameinuse(SERVERURL, client->getNickname(), message.parameters[0]));
	else if (__isValidNick(message.parameters[0])) // RPL_432_err_erroneusnickname
		client->addWrite_buffer(_432_err_erroneusnickname(SERVERURL, client->getNickname(), message.parameters[0]));
	else
	{
		// 동작 timestamp
		client->setNickname(message.parameters[0]);
		// client가 소속된 channel의 모든 유저에게 전송
		// current client의 channel size만큼 반복
		std::vector<Channel *> &channels = client->getChannels();
		for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			// 해당 channel의 send_msg 제작
			// channel은 user size만큼 반복
			Channel *curr_ch = *it;
			std::vector<Client *> client_list = curr_ch->getUsers();
			for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
			{
				// 각 클라이언트의 fd를 저장
				// 각 클라이언트의 send_buffer에 send_msg를 이어붙이기 (add)
				if ((*cl_it)->isAlive())
					fds.push_back((*cl_it)->getFd());
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " NICK " + message.parameters[0] + "\r\n");
			}
		}
		// 해당 유저들의 fd를 t_send_event에 넣는다
		_setSendEvent(true, true, false, true, fds);
	}
	return (SUCCESS);
}


int	Irc::__cmd_ping(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
	{
		_setSendEvent(true, true, false, true, fds);
		if (message.parameters.size() == 1)
			// 동작
			client->addWrite_buffer(client->makeClientPrefix() + " PONG " + SERVERNAME + " :" + message.parameters[0] + "\r\n");
			// :prifix PONG server :message.parameters
		else
			// 동작
			client->addWrite_buffer(client->makeClientPrefix() + " PONG " + message.parameters[1] + " :" + message.parameters[0] + "\r\n");
			// :prifix PONG message.parameters[1] :message.parameters[0]
	}
	return (SUCCESS);
}

int Irc::__cmd_pong(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
	{
		_setSendEvent(true, false, false, true, fds);
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	}
	else
		//timestamp
		_setSendEvent(true, true, false, false, fds);
	return (SUCCESS);
}

int Irc::__cmd_list(Client *client, IRCMessage message)
{
	(void)message; // 만약 끝까지 필요없다면 인자 자체 삭제 요망
	std::vector<int> fds;
	// 자기 자신에게 전송
	if (client->isAlive())
		fds.push_back(client->getFd());
	// 무조건 동작 timestamp
	_setSendEvent(true, true, false, true, fds);
	// RPL_321_rpl_liststart
	client->addWrite_buffer(_321_rpl_liststart(SERVERURL, client->getNickname()));
	for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		int cnt;

		cnt = 0;
		std::vector<Client *> client_list = (it)->getUsers();
		for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
			cnt++;
		std::stringstream ss;
		ss << cnt;
		std::string user_num = ss.str();
		// RPL_322_rpl_list
		client->addWrite_buffer(_322_rpl_list(SERVERURL, client->getNickname(), (it)->getName(), user_num, (it)->getTopic()));	
	}
	// RPL_323_rpl_listend
	client->addWrite_buffer(_323_rpl_listend(SERVERURL, client->getNickname()));
	return (SUCCESS);
}

int Irc::__cmd_who(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL_461_err_needmoreparams
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (!__isValidChannelName(message.parameters[0]))
		client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
	else if (!isExistingChannel(message.parameters[0]))
		client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), message.parameters[0]));
	else
	{
		std::vector<Channel>::iterator iter;
		for (iter = channels.begin(); iter != channels.end() ; iter++)
			if (iter->getName() == message.parameters[0])
				break ;
		std::vector<Client *> client_list = iter->getUsers();
		for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
		{
			// RPL_352_rpl_whoreply
			if (iter->isOperator((*cl_it)->getNickname()))
				client->addWrite_buffer(_352_rpl_whoreply(SERVERURL, client->getNickname(), message.parameters[0], (*cl_it)->getUsername(), "@", (*cl_it)->getRealname()));
			else
				client->addWrite_buffer(_352_rpl_whoreply(SERVERURL, client->getNickname(), message.parameters[0], (*cl_it)->getUsername(), "", (*cl_it)->getRealname()));
		}
		// RPL_315_rpl_endofwho
		client->addWrite_buffer(_315_rpl_endofwho(SERVERURL, client->getNickname(), message.parameters[0]));
		_setSendEvent(true, true, false, true, fds);
	}
	return (SUCCESS);
}

int Irc::__cmd_quit(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	std::vector<Channel *> &channels = client->getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *curr_ch = *it;
		std::vector<Client *> client_list = curr_ch->getUsers();
		for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
	// 해당 유저들의 fd를 t_send_event에 넣는다
			if ((*cl_it)->getFd() != client->getFd())
			{
				if ((*cl_it)->isAlive())
					fds.push_back((*cl_it)->getFd());
				if (message.parameters.size() == 0)
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " QUIT :Quit: " + QUITMSG + "\r\n");
				else
				{
					std::vector<std::string>::iterator iter = message.parameters.begin();
					std::string msg = *iter;
					iter++;
					for (; iter != message.parameters.end() ; iter++)
					{
						msg = msg + " " + *iter;
					}
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " QUIT :Quit: " + msg + "\r\n");
				}
			}
	}
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	_setSendEvent(true, false, true, true, fds);
	client->setLife(false);
	return (SUCCESS);
}

// 사실상 서버 자체 봇 설계가 여기 들어가게됨
int Irc::__cmd_privmsg(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
	{
		std::vector<std::string> targets;
		// 첫번째 파라미터를 ','를 기준으로 분리
		targets = __getTargets(message.parameters[0]);
		// loop
		std::vector<std::string>::iterator target_iter;
		for (target_iter = targets.begin() ; target_iter != targets.end() ; target_iter++)
			std::cerr << "[" << *target_iter << "]\n";
		for (target_iter = targets.begin() ; target_iter != targets.end() ; target_iter++)
		{
			if (__isValidChannelName(*target_iter))
			{
				if (!__isValidChannelName(*target_iter)) // RPL 476
					client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), *target_iter));
				else if (!isExistingChannel(*target_iter)) // RPL 403
					client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), *target_iter));
//				else if ((채널 모드 n이 true) && (채널 소속이 아님)) 채널의 모드 중 n이 존재하지 않으므로 주석처리 됨
//					client->addWrite_buffer(_442_err_notonchannel(SERVERURL, client->getNickname(), *target_iter));
				else
				{
					// 동작
					Channel *chan = searchChannel(*target_iter);
					// 메세지 작성하기
					std::string reply_msg;

					std::vector<std::string>::iterator param_iter = message.parameters.begin();
					param_iter++;
					std::string msg = *param_iter;
					param_iter++;
					for (; param_iter != message.parameters.end() ; param_iter++)
					{
						msg = msg + " " + *param_iter;
					}

					// if (메세지 내용이 BOTNAME" COMMAND") // BOTNAME
					// {
					// 	if (chan->isUser(BOTNAME))
					// 	{
					// 		Client *bot = chan->getBot();
					// 		// bot 동작시키기 == 미리 설정해둔 대답 꺼내오기
					// 		std::string bot_msg = "cmd1 cmd2 cmd3 ...";
					// 		reply_msg = bot->makeClientPrefix() + " PRIVMSG " + *target_iter + " :" + bot_msg + "\r\n";
					// 	}
					// }
					// else
					{
						reply_msg = client->makeClientPrefix() + " PRIVMSG " + *target_iter + " :" + msg + "\r\n";
					}
					std::vector<Client *> client_list = chan->getUsers();
					for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
					{
						// 각 클라이언트의 fd를 저장
						// 각 클라이언트의 send_buffer에 send_msg를 이어붙이기 (add)
						if ((*cl_it)->isAlive())
							fds.push_back((*cl_it)->getFd());
						client->addWrite_buffer(reply_msg);
					}
					_setSendEvent(true, true, false, true, fds);
					fds.clear();
				}
			}
			else
			{
				if (__isValidNick(*target_iter)) // RPL_432_err_erroneusnickname
					client->addWrite_buffer(_432_err_erroneusnickname(SERVERURL, client->getNickname(), *target_iter));
				else
				{
					// 동작
					Client *target_client = searchClient(*target_iter);
					if (target_client == NULL)
						client->addWrite_buffer(_401_err_nosuchnick(SERVERURL, client->getNickname(), *target_iter));

					std::vector<std::string>::iterator param_iter = message.parameters.begin();
					param_iter++;
					std::string msg = *param_iter;
					param_iter++;
					for (; param_iter != message.parameters.end() ; param_iter++)
					{
						msg = msg + " " + *param_iter;
					}

					if (target_client->isAlive())
							fds.push_back(target_client->getFd());
					client->addWrite_buffer(client->makeClientPrefix() + " PRIVMSG " + *target_iter + " :" + msg + "\r\n");
					_setSendEvent(true, true, false, true, fds);
					fds.clear();
				}
			}
		}
	}
	return (SUCCESS);
}

int Irc::__cmd_join(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() < 1) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
	{
		std::vector<std::string> targets;
		std::vector<std::string> keys;
		// 첫번째 파라미터를 ','를 기준으로 분리하여 vector에 넣기
		targets = __getTargets(message.parameters[0]);
		// 두번째 파라미터 param[1]을 ','를 기준으로 분리하여 vector에 넣기
		if (message.parameters.size() > 1)
		keys = __getTargets(message.parameters[1]);
		std::vector<std::string>::iterator key_iter;
		// loop
		std::vector<std::string>::iterator chan_iter;
		key_iter = keys.begin();
		for (chan_iter = targets.begin() ; chan_iter != targets.end() ; chan_iter++)
		{
			if (!__isValidChannelName(*chan_iter))
				client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), *chan_iter));
			else if (!isExistingChannel(*chan_iter)) // 존재하지 않는 채널
			{
				// 존재하지 않는 채널
				// 채널 생성
				// op로서 가입
				{
					Channel new_channel;
					new_channel.setName(*chan_iter);
					new_channel.addOperator(client);
					new_channel.addUser(client);
					this->channels.push_back(new_channel);
					client->addChannel(searchChannel(*chan_iter));
				}
				client->addWrite_buffer(client->makeClientPrefix() + " JOIN :" + *chan_iter + "\r\n");
				_setSendEvent(true, true, false, true, fds);
				fds.clear();
			}
			else // 존재하는 채널
			{
				// 채널 탐색 및 값 가져오기
				Channel *chan = searchChannel(*chan_iter);
				if (client->isMaxJoin()) // RPL_405_err_toomanychannels
					client->addWrite_buffer(_405_err_toomanychannels(SERVERURL, client->getNickname(), *chan_iter));	
				else if (chan->getOptionInvite() && !chan->isInvite(client->getNickname())) // RPL_473_err_inviteonlychan
					client->addWrite_buffer(_473_err_inviteonlychan(SERVERURL, client->getNickname(), *chan_iter));
				else if (chan->getOptionkey() && (key_iter == keys.end() || !chan->isKey(*key_iter))) // RPL_475_err_badchannelkey
					client->addWrite_buffer(_475_err_badchannelkey(SERVERURL, client->getNickname(), *chan_iter));
				else if (chan->getOptionLimit() && chan->isFull()) // RPL_471_err_channelisfull
					client->addWrite_buffer(_471_err_channelisfull(SERVERURL, client->getNickname(), *chan_iter));
				else
				{
					// 일반 유저로 가입
					std::vector<Client *> client_list = chan->getUsers();
					for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
					{
						// 각 클라이언트의 fd를 저장
						// 각 클라이언트의 send_buffer에 send_msg를 이어붙이기 (add)
						if ((*cl_it)->isAlive())
							fds.push_back((*cl_it)->getFd());
						client->addWrite_buffer(client->makeClientPrefix() + " JOIN :" + *chan_iter + "\r\n");
					}
					// 채널에 유저 추가
					chan->addUser(client);
					chan->delInvite(client->getNickname());
					_setSendEvent(true, true, false, true, fds);
					fds.clear();
				}
			}
			if (key_iter != keys.end())
				key_iter++;
		}
	}
	return (SUCCESS);
}
/*
join #aa
:nick!uuuu@127.0.0.1 JOIN :#aa
:penguin.omega.example.org 353 nick = #aa :@nick
:penguin.omega.example.org 366 nick #aa :End of /NAMES list.
:kiryud!jeongjinse@127.0.0.1 JOIN :#aa
:jijeong!jeongjinse@127.0.0.1 JOIN :#aa
*/

int Irc::__cmd_part(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, true, true, fds);

	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
	{
		std::vector<std::string> targets;
		// 첫번째 파라미터를 ','를 기준으로 분리하여 vector에 넣기
		targets = __getTargets(message.parameters[0]);
		// loop
		std::vector<std::string>::iterator chan_iter;
		for (chan_iter = targets.begin() ; chan_iter != targets.end() ; chan_iter++)
		{
			if (!__isValidChannelName(*chan_iter)) // RPL 476
				client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), *chan_iter));
			else if (!isExistingChannel(*chan_iter)) // RPL 403
				client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), *chan_iter));
			else
			{
				if (client->isChannel(*chan_iter))
				{
					// 동작 - message 제작
					std::vector<std::string>::iterator param_iter = message.parameters.begin();
					std::string msg = *param_iter;
					param_iter++;
					for (; param_iter != message.parameters.end() ; param_iter++)
						msg = msg + " " + *param_iter;
					// 동작 - channel 찾아오기
					Channel *chan = searchChannel(*chan_iter);
					// 채절의 모든 유저에게 나간다고 알림
					std::vector<Client *> client_list = chan->getUsers();
					for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
					{
						// 각 클라이언트의 fd를 저장
						// 각 클라이언트의 send_buffer에 send_msg를 이어붙이기 (add)
						if ((*cl_it)->isAlive())
							fds.push_back((*cl_it)->getFd());
						(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " PART " + *chan_iter + " :" + msg + "\r\n");
					}
					// 채널에서 삭제
					chan->delOperator(client->getNickname());
					chan->delUser(client->getNickname());
					client->delChannel(chan->getName());
					_setSendEvent(true, true, false, true, fds);
					fds.clear();
				}
				else // RPL
					client->addWrite_buffer(_442_err_notonchannel(SERVERURL, client->getNickname(), *chan_iter));
			}
		}
	}
	return (SUCCESS);
}

int Irc::__cmd_topic(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (!__isValidChannelName(message.parameters[0]))
		client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
	else if (!isExistingChannel(message.parameters[0]))
		client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), message.parameters[0]));
	else
	{
		std::vector<Channel>::iterator iter;
			for (iter = channels.begin(); iter != channels.end() ; iter++)
				if (iter->getName() == message.parameters[0])
					break ;
		if (message.parameters.size() == 1)
		{
			std::string topic = iter->getTopic();
			if (topic.empty()) // RPL 331
				client->addWrite_buffer(_331_rpl_notopic(SERVERURL, client->getNickname(), message.parameters[0]));
			else // RPL 332
				client->addWrite_buffer(_332_rpl_topic(SERVERURL, client->getNickname(), message.parameters[0], topic));
			_setSendEvent(true, true, false, true, fds);
		}
		else
		{
			if (!iter->isOperator(client->getNickname())) // RPL 482
				client->addWrite_buffer(_482_err_chanoprivsneeded(SERVERURL, client->getNickname(), message.parameters[0]));
			else
			{
				// 동작 - timestamp
				std::vector<std::string>::iterator param_iter = message.parameters.begin();
				param_iter++;
				std::string topic = *param_iter;
				param_iter++;
				for (; param_iter != message.parameters.end() ; param_iter++)
				{
					topic = topic + " " + *param_iter;
				}
				iter->setTopic(topic);
				// 해당 channel의 모든 client에게 전송
				std::vector<Client *> client_list = iter->getUsers();
				for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
				{
					if ((*cl_it)->isAlive())
						fds.push_back((*cl_it)->getFd());
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " TOPIC " + iter->getName() + " :" + topic + "\r\n");
				}
				_setSendEvent(true, true, false, true, fds);
			}
		}
	}
	return (SUCCESS);
}

int Irc::__cmd_kick(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (!__isValidChannelName(message.parameters[0])) // RPL 476
		client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
	else if (!isExistingChannel(message.parameters[0])) // RPL 403
		client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), message.parameters[0]));
	else
	{
		std::vector<Channel>::iterator iter;
		for (iter = channels.begin(); iter != channels.end() ; iter++)
			if (iter->getName() == message.parameters[0])
				break ;
		if (!iter->isUser(client->getNickname())) // RPL 442
			client->addWrite_buffer(_442_err_notonchannel(SERVERURL, client->getNickname(), message.parameters[0]));
		else if (!iter->isUser(message.parameters[1])) // RPL 401
			client->addWrite_buffer(_401_err_nosuchnick(SERVERURL, client->getNickname(), message.parameters[1]));
		else if (!iter->isOperator(client->getNickname())) // RPL 482
			client->addWrite_buffer(_482_err_chanoprivsneeded(SERVERURL, client->getNickname(), message.parameters[0]));
		else
		{
			Client *tmp_client;
			std::vector<std::string>::iterator param_iter = message.parameters.begin();
			param_iter++;
			param_iter++;
			std::string msg = *param_iter;
			for (; param_iter != message.parameters.end() ; param_iter++)
			{
				msg = msg + " " + *param_iter;
			}
			// 동작 - timestamp
			// 해당 channel의 모든 유저에게 전송
			std::vector<Client *> client_list = iter->getUsers();
			for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
			{
				if ((*cl_it)->getNickname() == message.parameters[1])
					tmp_client = *cl_it;
				if ((*cl_it)->isAlive())
					fds.push_back((*cl_it)->getFd());
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + " KICK " + iter->getName() + " :" + msg + "\r\n");
			}
			_setSendEvent(true, true, false, true, fds);
			// 해당 유저를 channel에서 제거
			iter->delOperator(message.parameters[1]);
			iter->delUser(message.parameters[1]);
			// 해당 유저에서 channel을 제거
			tmp_client->delChannel(message.parameters[0]);
		}
	}
	return (SUCCESS);
}

int Irc::__cmd_invite(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (!__isValidChannelName(message.parameters[0])) // RPL 476
		client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
	else if (!isExistingChannel(message.parameters[0])) // RPL 403
		client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), message.parameters[0]));
	else
	{
		std::vector<Channel>::iterator iter;
		for (iter = channels.begin(); iter != channels.end() ; iter++)
			if (iter->getName() == message.parameters[0])
				break ;
		if (!iter->isUser(client->getNickname())) // RPL 442
			client->addWrite_buffer(_442_err_notonchannel(SERVERURL, client->getNickname(), message.parameters[0]));
		else if (!iter->isUser(message.parameters[1])) // RPL 401
			client->addWrite_buffer(_401_err_nosuchnick(SERVERURL, client->getNickname(), message.parameters[1]));
		else if (!iter->isOperator(client->getNickname())) // RPL 482
			client->addWrite_buffer(_482_err_chanoprivsneeded(SERVERURL, client->getNickname(), message.parameters[0]));
		else
		{
			// 동작 - timestamp
			// 해당 channel의 모든 유저에게 전송
			std::vector<Client *> client_list = iter->getUsers();
			for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
			{
				if ((*cl_it)->isAlive())
					fds.push_back((*cl_it)->getFd());
				(*cl_it)->addWrite_buffer(_341_rpl_inviting(SERVERURL, client->getNickname(), iter->getName(), message.parameters[1]));
			}
			// 해당 유저에게 초대 메세지 보내기
			Client *tmp_client = searchClient(message.parameters[1]);
			if ((tmp_client)->isAlive())
				fds.push_back((tmp_client)->getFd());
			(tmp_client)->addWrite_buffer(client->makeClientPrefix() + " INVITE " + tmp_client->getNickname() + " :" + message.parameters[0] + "\r\n");
			// 해당 유저를 channel invited에 추가
			iter->addInvite(tmp_client);
			_setSendEvent(true, true, false, true, fds);
		}
	}
	return (SUCCESS);
}
// 초대받은 유저 :kiryud!jeongjinse@127.0.0.1 INVITE nick :#qwer
// 채널 소속 유저 :penguin.omega.example.org 341 kiryud nick :#qwer

int Irc::__cmd_mode(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);

	if (message.parameters.size() == 0) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
	{
		if (!__isValidChannelName(message.parameters[0])) // RPL 476
			client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
		else if (!isExistingChannel(message.parameters[0])) // RPL 403
			client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), message.parameters[0]));
		else
		{
			// 채널 탐색 및 값 가져오기
			Channel *chan = searchChannel(message.parameters[0]);
			if (message.parameters.size() == 1)
			{
				std::string channelMode = chan->getMode(); // 이건 아무나 볼 수 있어도 되는디
				std::string channelModeParam = chan->getModeParam(client->getNickname()); // 여기서 비밀번호는 채널 소속만 봐야함 ㅇㅁㅇ
				// 채널 유저 맞음 :penguin.omega.example.org 324 jijeong #asdf +klnt 123243546 :10
				// 채널 유저 아님 :penguin.omega.example.org 324 kiryud #asdf +klnt <key> :10
				// key false일 때 :penguin.omega.example.org 324 kiryud #asdf +klnt :10
				client->addWrite_buffer(_324_rpl_channelmodeis(SERVERURL, client->getNickname(), chan->getName(), channelMode, channelModeParam));
				_setSendEvent(true, true, false, true, fds);
			}
			else
			{
				if (!chan->isUser(client->getNickname())) // RPL 442
					client->addWrite_buffer(_442_err_notonchannel(SERVERURL, client->getNickname(), message.parameters[0]));
				else if (!chan->isOperator(client->getNickname())) // RPL 482
					client->addWrite_buffer(_482_err_chanoprivsneeded(SERVERURL, client->getNickname(), message.parameters[0]));
				else
				{
					// 여기서 루프 타는거 어때??
					// mode param은 param index 2부터 시작하고 param 관련 옵션이라 호출할때만 +
					// TITLE, KEY, LIMIT, INVITE + op 다섯가지는 어차피 문자로 들어오는거니 switch도 가능
					// 그럼 default가 _472_err_unknownmode
					// 그리고 실행 가능한데 param 못 꺼내오면 (end() 일 때) _696_err_invalidmodeparam
					// 각 경우 마지막에 성공한다면 channel에게 아니면 본인에게인거니까 ㅇㅇ
					std::string options = message.parameters[1];
					std::vector<std::string>::iterator param_iter;
					param_iter = message.parameters.begin();
					std::advance(param_iter, 2);
					std::string::iterator opt_iter = options.begin();
					bool flag;
					if (*opt_iter == '+')
					{
						flag = true;
						opt_iter++;
					}
					else if (*opt_iter == '-')
					{
						flag = false;
						opt_iter++;
					}
					else
						flag = true;

					for (; opt_iter != options.end() ; opt_iter++)
					{
						int i;
						char option[] = {'t', 'k', 'l', 'i', 'o'};
						for (i = 0; i < 5 ; i++)
							if (*opt_iter == option[i])
								break ;

						switch (i)
						{
							case 0: // topic
								if (!(flag == chan->getOptionTitle()))
								{
									chan->setOptionTitle(flag);
									std::string rplmsg;
									if (flag)
									{
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " :+t" + "\r\n";
									}
									else
									{
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " :-t" + "\r\n";
									}
									std::vector<Client *> client_list = chan->getUsers();
									for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
									{
										if ((*cl_it)->isAlive())
											fds.push_back((*cl_it)->getFd());
										(*cl_it)->addWrite_buffer(rplmsg);
									}
									_setSendEvent(true, true, false, true, fds);
								}
								break ;
							case 1: // key
								if (param_iter == message.parameters.end())
									client->addWrite_buffer(_696_err_invalidmodeparam(SERVERURL, client->getNickname(), "k"));
								else if (!(flag == chan->getOptionkey()))
								{
									if (!flag && (*param_iter).compare(chan->getPassword()))
										client->addWrite_buffer(_467_err_keyset(SERVERURL, client->getNickname(), chan->getName()));
									else
									{
										chan->setOptionTitle(flag);
										std::string rplmsg;
										if (flag)
										{
											chan->setPassword(*param_iter);
											rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " +k :" + *param_iter + "\r\n";
										}
										else
										{
											chan->delPassword();
											rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " -k :" + *param_iter + "\r\n";
										}
										std::vector<Client *> client_list = chan->getUsers();
										for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
										{
											if ((*cl_it)->isAlive())
												fds.push_back((*cl_it)->getFd());
											(*cl_it)->addWrite_buffer(rplmsg);
										}
										_setSendEvent(true, true, false, true, fds);
									}
								}
								param_iter++;
								break ;
							case 2: // limit
								if (param_iter == message.parameters.end())
									client->addWrite_buffer(_696_err_invalidmodeparam(SERVERURL, client->getNickname(), "l"));
								else if (!(flag == chan->getOptionLimit()))
								{
									double num = std::strtod((*param_iter).c_str(), NULL);
									chan->setLimit(static_cast<size_t>(num));
									chan->setOptionLimit(flag);
									std::string rplmsg;
									if (flag)
									{
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " +l :" + *param_iter + "\r\n";
									}
									else
									{
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " -l :" + *param_iter + "\r\n";
									}
									std::vector<Client *> client_list = chan->getUsers();
									for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
									{
										if ((*cl_it)->isAlive())
											fds.push_back((*cl_it)->getFd());
										(*cl_it)->addWrite_buffer(rplmsg);
									}
									_setSendEvent(true, true, false, true, fds);
								}
								param_iter++;
								break ;
							case 3: // invite
								if (!(flag == chan->getOptionInvite()))
								{
									chan->setOptionInvite(flag);
									std::string rplmsg;
									if (flag)
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " :+i" + "\r\n";
									else
										rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " :-i" + "\r\n";
									std::vector<Client *> client_list = chan->getUsers();
									for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
									{
										if ((*cl_it)->isAlive())
											fds.push_back((*cl_it)->getFd());
										(*cl_it)->addWrite_buffer(rplmsg);
									}
									_setSendEvent(true, true, false, true, fds);
								}
								break ;
							case 4: // op
								if (param_iter == message.parameters.end())
									client->addWrite_buffer(_696_err_invalidmodeparam(SERVERURL, client->getNickname(), "o"));
								else if (!__isValidChannelName(message.parameters[0])) // RPL 476
									client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), message.parameters[0]));
								else if (!chan->isUser(*param_iter))
									client->addWrite_buffer(_401_err_nosuchnick(SERVERURL, client->getNickname(), *param_iter));
								else if (!chan->isOperator(*param_iter))
								{
									chan->addOperator(searchClient(*param_iter));
									std::string rplmsg;
									rplmsg = client->makeClientPrefix() + " MODE " + chan->getName() + " +o :" + client->getNickname() + "\r\n";
									std::vector<Client *> client_list = chan->getUsers();
									for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
									{
										if ((*cl_it)->isAlive())
											fds.push_back((*cl_it)->getFd());
										(*cl_it)->addWrite_buffer(rplmsg);
									}
									_setSendEvent(true, true, false, true, fds);
								}
								param_iter++;
								break ;
							default :
								std::string tmp;
								tmp += *opt_iter;
								client->addWrite_buffer(_472_err_unknownmode(SERVERURL, client->getNickname(), tmp));
						}
					}
				}
			}
		}
	}
	return (SUCCESS);
}
/*
Tue Feb 06 2024 19:47:09 USERINPUT: C[422AAAAAA] I MODE #asdf +o asdf
Tue Feb 06 2024 19:47:09 USEROUTPUT: C[422AAAAAA] O :penguin.omega.example.org 401 jijeong asdf :No such nick
*/
/*
Tue Feb 06 2024 19:47:19 USERINPUT: C[422AAAAAA] I MODE #asdf +o jijeong
Tue Feb 06 2024 19:47:25 USERINPUT: C[422AAAAAA] I MODE #asdf +o kiryud
Tue Feb 06 2024 19:47:25 USEROUTPUT: C[422AAAAAA] O :jijeong!jeongjinse@localhost MODE #asdf +o :kiryud
Tue Feb 06 2024 19:47:25 USEROUTPUT: C[422AAAAAB] O :jijeong!jeongjinse@localhost MODE #asdf +o :kiryud
*/

int	Irc::__not_a_command(Client *client, IRCMessage message)
{
	// RPL_421_err_unknowncommand
	std::vector<int> fds;
	fds.push_back(client->getFd());
	_setSendEvent(true, false, false, true, fds);
	client->addWrite_buffer(_421_err_unknowncommand(SERVERURL, client->getNickname(), message.command));
	return (SUCCESS);
}
