#include "Irc.hpp"
#include <iostream> /* test header */
#include <sstream>

Irc::Irc(void) : password(NULL), clients(NULL), channels(NULL)
{
	commands = {"USER", "PASS", "NICK", "PING", "PONG", "LIST", "WHO", "QUIT", "PRIVMSG", "JOIN", "PART", "TOPIC", "KICK", "INVITE", "MODE"};
}

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
		// 만약 문자열이 공백이라면 loop 종료
		if (commandLine.empty())
			break ;
		// 리시브 메세지 해석
		IRCMessage recv_msg = parseMessage(recv_buffer);
		// 명령어에 따라 동작하기
		if (!client->isRegistered())
			_register_executor(client, recv_msg);
		_command_executor(client, recv_msg);
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
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "QUIT :Quit: " + msg);
			}
	}
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	_setSendEvent(true, false, true, true, fds);
	client->setLife(false);
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
	std::vector<int> fds;

	send_msg.recv_work;
	send_msg.recv_time;
	send_msg.recv_close;
	send_msg.to_send;
	send_msg.fds = fds;
	return (SUCCESS);
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
		std::vector<int> fds(client->getFd());
		_setSendEvent(false, false, false, true, fds);
		client->addWrite_buffer(_451_err_notregistered(SERVERURL, client->getNickname()));
	}
	if (client->isRegistered()) // RPL_001_rpl_welcome
	{
		std::vector<int> fds(client->getFd());
		_setSendEvent(false, true, false, true, fds);
		client->addWrite_buffer(_001_rpl_welcome(SERVERURL, client->getNickname(), client->makeClientPrefix()));
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
	std::vector<std::string>::iterator iter;
	for (iter = commands.begin() ; iter != commands.end() ; std::advance(iter, 1))
	{
		if (cmd == *iter)
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
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "NICK " + message.parameters[0]);
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
			client->addWrite_buffer(client->makeClientPrefix() + "PONG " + SERVERNAME + " :" + message.parameters[0]);
			// :prifix PONG server :message.parameters
		else
			// 동작
			client->addWrite_buffer(client->makeClientPrefix() + "PONG " + message.parameters[1] + " :" + message.parameters[0]);
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
	std::vector<int> fds;
	// 자기 자신에게 전송
	if (client->isAlive())
		fds.push_back(client->getFd());
	// 무조건 동작 timestamp
	_setSendEvent(true, true, false, true, fds);
	// RPL_321_rpl_liststart
	client->addWrite_buffer(_321_rpl_liststart(SERVERURL, client->getNickname()));
	std::vector<Channel *> &channels = client->getChannels();
	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		Channel *curr_ch = *it;
		int cnt;

		cnt = 0;
		std::vector<Client *> client_list = curr_ch->getUsers();
		for (std::vector<Client *>::iterator cl_it = client_list.begin(); cl_it != client_list.end(); cl_it++)
			cnt++;
		std::stringstream ss;
		ss << cnt;
		std::string user_num = ss.str();
		// RPL_322_rpl_list
		client->addWrite_buffer(_322_rpl_list(SERVERURL, client->getNickname(), (*it)->getName(), user_num, (*it)->getTopic()));	
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
	_setSendEvent(true, false, true, true, fds);
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
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "QUIT :Quit: " + QUITMSG);
				else
				{
					std::vector<std::string>::iterator iter = message.parameters.begin();
					std::string msg = *iter;
					iter++;
					for (; iter != message.parameters.end() ; iter++)
					{
						msg = msg + " " + *iter;
					}
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "QUIT :Quit: " + msg);
				}
			}
	}
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	_setSendEvent(true, false, true, true, fds);
	client->setLife(false);
	return (SUCCESS);
}

int Irc::__cmd_privmsg(Client *client, IRCMessage message)
{
	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
		// 첫번째 파라미터를 ','를 기준으로 분리
		// loop
			if (#으로 시작하지 않음)
				if (nick일 수 없음)
					//
				else if (nick을 가진 유저가 없음)
					//
				else
					// 동작
			else if (#으로 시작함)
				if (채널 이름일수가 없음)
					// RPL 476
				else if (채널이 없음)
					// RPL 403
				else if (채널 소속이 아님 이거 MODE +n임 ㅇㅁㅇ)
					// RPL
				else
					// 동작
					if (메세지 내용이 ":/BOTNAME COMMAND")
						// bot 동작시키기
	return (SUCCESS);
}

int Irc::__cmd_join(Client *client, IRCMessage message)
{
	if (message.parameters.size() < 2) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else
		// 첫번째 파라미터를 ','를 기준으로 분리
		// loop
			if (채널 이름일수가 없음)
				// RPL 476
			else if (채널 탐색 실패)
				// RPL 403
			else if (+i 초대 못받음)
				// RPL 473
			else if (+k 초대 못받음)
				// RPL 475
			else if (+l 초대 못받음)
				// RPL 471
			else if (client가 channel 가입 상한 초과)
				// RPL 405
			else if (존재하는 채널)
				// 일반 유저로 가입
			else if (존재하지 않는 채널)
				// 채널 생성
				// op로서 가입
	return (SUCCESS);
}

int Irc::__cmd_part(Client *client, IRCMessage message)
{
	if (message.parameters.size() < 2)
		// RPL 461
	else
		// 첫번째 파라미터를 ','를 기준으로 분리
		// loop
			if (채널 이름일수가 없음)
				// RPL 476
			else if (채널 탐색 실패)
				// RPL 403
			else if (채널 탐색 성공)
				if (가입중)
					// 동작
					// 채절의 모든 유저에게 나간다고 알림
					// 채널에서 삭제
				else
					// RPL
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
					(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "TOPIC " + iter->getName() + " " + topic);
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

	if (message.parameters.size() == 0) // RPL 461
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
				(*cl_it)->addWrite_buffer(client->makeClientPrefix() + "KICK " + iter->getName() + " " + msg);
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
	if (message.parameters.size() == 0) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (채널 이름일수가 없음)
		// RPL 476
	else if (채널 탐색 실패)
		// RPL 403
	else if (해당 클라이언트는 채널의 멤버가 아님)
		// RPL 442
	else if (nick을 가진 클라이언트가 없음)
		// RPL 401
	else if (client는 해당 채널의 op가 아님)
		// RPL 482
	else
		// 동작 - timestamp
		// 해당 channel의 모든 유저에게 전송
		// 해당 유저를 channel에서 제거
	return (SUCCESS);
}

int Irc::__cmd_mode(Client *client, IRCMessage message)
{
	std::string channelName = message.parameters[0];
	Channel *channel;
	std::vector<int> fds(client->getFd());
	_setSendEvent(true, false, false, true, fds);
	if (message.parameters.size() == 0) // RPL 461
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
	else if (message.parameters.size() == 1)
	{
		if (채널 이름일수가 없음)
			// RPL 476
		else
		{
			// 채널 탐색 및 값 가져오기
			std::string channelMode;
			std::string channelModeParam;
			if (채널 탐색 실패) // RPL 403
				client->addWrite_buffer(_403_err_nosuchchannel(SERVERURL, client->getNickname(), channelName));
			else // RPL 324
				client->addWrite_buffer(_324_rpl_channelmodeis(SERVERURL, client->getNickname(), channel->getName(), channelMode, channelModeParam));
		}
	}
	else if (채널 이름일수가 없음) // RPL 476
		client->addWrite_buffer(_476_err_badchanmask(SERVERURL, client->getNickname(), channelName));
	else
	{
		if (채널 탐색 실패)
			// RPL 403
		else if (client는 해당 채널의 op가 아님)
			// RPL 482
		else if (모르는 char가 들어온 경우) // RPL 472 경우마다지만 한개만 보내 보기
			client->addWrite_buffer(_472_err_unknownmode(SERVERURL, client->getNickname(), modes));
		else if (o k l 의 param이 부족한 경우) // RPL 696 경우마다지만 한개만 보내 보기
			client->addWrite_buffer(_696_err_invalidmodeparam(SERVERURL, client->getNickname(), channelName));
		else if (key 이미 설정됨) // RPL 467
			client->addWrite_buffer(_467_err_keyset(SERVERURL, client->getNickname(), channelName));
		else
			// 동작 - timestamp
			_setSendEvent(true, true, false, true, fds);
			// 해당 channel의 모든 유저에게 전송
			
	}
	return (SUCCESS);
}

int	Irc::__not_a_command(Client *client, IRCMessage message)
{
	// RPL_421_err_unknowncommand
	std::vector<int> fds(client->getFd());
	_setSendEvent(true, false, false, true, fds);
	client->addWrite_buffer(_421_err_unknowncommand(SERVERURL, client->getNickname(), message.command));
	return (SUCCESS);
}
