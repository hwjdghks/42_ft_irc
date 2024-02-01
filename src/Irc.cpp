#include "Irc.hpp"
#include <iostream> /* test header */

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
	std::vector<int> empty;
	_setSendEvent(false, false, false, false, empty);
	// 해당 fd의 클라이언트 찾아오기
	Client *client = searchClient(fd);
	// 메세지 리드버퍼에 저장
	// 리드버퍼 해석 <개행 찾기>
	// 없으면 종료
	// 있다면 그 개수만큼 loop
	// 리시브 메세지 해석
	IRCMessage recv_msg;
	// 명령어에 따라 동작하기
	if (!client->isRegistered())
		_register_executor(client, recv_msg);
	_command_executor(client, recv_msg);
	return (send_msg);
}

Client *Irc::searchClient(int fd)
{
	Client *client;
	// client vector를 순회하여 fd에 해당하는 클라이언트 가져오기
	return (client);
}

t_send_event Irc::ping(int fd)
{
	// t_send_event에 fd를 넣는다
	// 해당 fd의 클라이언트를 찾는다
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	return (send_msg);
}

t_send_event Irc::quit(int fd, const char *msg)
{
	// 해당 fd의 클라이언트를 찾는다
	// 해당 클라이언트가 속한 채널의 유저들을 찾는다
	// 해당 유저들의 fd를 t_send_event에 넣는다
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	return (send_msg);
}

t_send_event Irc::deleteClient(int fd)
{
	// 해당 fd의 클라이언트를 찾는다
	// 해당 클라이언트가 가입된 채널을 찾아 제거한다
	// 클라이언트 벡터에서 해당 클라이언트를 제거한다.
	return (send_msg);
}

int	Irc::_setSendEvent(bool recv_work, bool recv_time, bool recv_close, bool to_send, std::vector<int> fds)
{
	send_msg.recv_work = recv_work;
	send_msg.recv_time = recv_time;
	send_msg.recv_close = recv_close;
	send_msg.to_send = to_send;
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
	else if (__isCommand(recv_msg.command))
		// RPL_451_err_notregistered
	if (client->isRegistered())
		// RPL_001_rpl_welcome
	return (SUCCESS);
}

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

bool Irc::__isCommand(std::string cmd)
{
	std::vector<std::string>::iterator iter;
	for (iter = commands.begin() ; iter != commands.end() ; std::advance(iter, 1))
	{
		if (cmd == *iter)
			(true)
	}
	return (false);
}

int Irc::_command_executor(Client *client, IRCMessage recv_msg)
{
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

int __cmd_user(Client *client, IRCMessage message)
{
	if (param 부족함)
		// RPL_461_err_needmoreparams
	else
		// RPL_462_err_alreadyregisterd
	return (SUCCESS);
}

int __cmd_pass(Client *client, IRCMessage message)
{
	if (param 부족함)
		// RPL_461_err_needmoreparams
	else
		// RPL_462_err_alreadyregisterd
	return (SUCCESS);
}

int __cmd_nick(Client *client, IRCMessage message)
{
	if (param 없음)
		// RPL_461_err_needmoreparams
	else if (이미 등록된 닉네임)
		// RPL_433_err_nicknameinuse
	else if (사용할 수 없는 문자 포함 ('#', ',', ' ', '@' 등))
		// RPL_432_err_erroneusnickname
	else
		// 동작
		// 자기 자신에게 전송
		// client가 소속된 channel의 모든 유저에게 전송
		// timestamp
	return (SUCCESS);
}


int	Irc::__cmd_ping(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL_461_err_needmoreparams
	else if (message.parameters.size() == 1)
		// 동작
		// :prifix PONG server :message.parameters
		// timestamp
	else
		// 동작
		// :prifix PONG message.parameters[1] :message.parameters[0]
		// timestamp
	return (SUCCESS);
}

int Irc::__cmd_pong(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL_461_err_needmoreparams
	else
		//timestamp
	return (SUCCESS);
}

int Irc::__cmd_list(Client *client, IRCMessage message)
{
	// 무조건 동작
	// 자기 자신에게 전송
	// RPL_321_rpl_liststart
	for (int i = 0; i < channels.size() ; i++)
		// RPL_322_rpl_list
	// RPL_323_rpl_listend
	//timestamp
	return (SUCCESS);
}

int Irc::__cmd_who(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL_461_err_needmoreparams
	else if (_is_nick_exist(message.parameters[0]))
		// RPL_352_rpl_whoreply
	else if (_is_channel_exist(message.parameters[0]))
		for (해당 채널의 유저를 순회하며 출력)
			// RPL_352_rpl_whoreply
	// RPL_315_rpl_endofwho
	// 자기 자신에게 출력
	// timestamp
	return (SUCCESS);
}

int Irc::__cmd_quit(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// 동작 : 메세지 공란
	else
		// 동작 : 메세지 같이 출력
	// 자기 자신에게 출력
	// client가 소속된 channel의 모든 유저에게 전송
	return (SUCCESS);
}

int Irc::__cmd_privmsg(Client *client, IRCMessage message)
{
	if (message.parameters.size() < 2)
		// RPL 461
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
	if (message.parameters.size() < 2)
		// RPL 461
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
	if (message.parameters.size() == 0)
		// RPL 461
	else if (message.parameters.size() == 1)
		if (채널 이름일수가 없음)
			// RPL 476
		else if (채널 탐색 실패)
			// RPL 403
		else if (topic != NULL)
			// RPL 332
		else
			// RPL 331
	else
		if (채널 탐색 실패)
			// RPL 403
		else if (client가 channel의 op가 아님)
			// RPL 482
		else
			// 동작 - timestamp
			// 해당 channel의 모든 client에게 전송
	return (SUCCESS);
}

int Irc::__cmd_kick(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL 461
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

int Irc::__cmd_invite(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL 461
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
	if (message.parameters.size() == 0)
		// RPL 461
	else if (message.parameters.size() == 1)
		if (채널 이름일수가 없음)
			// RPL 476
		else if (채널 탐색 실패)
			// RPL 403
		else
			// RPL 324
	else if (채널 이름일수가 없음)
		// RPL 476
	else if (채널 탐색 실패)
		// RPL 403
	else if (client는 해당 채널의 op가 아님)
		// RPL 482
	else if (모르는 char가 들어온 경우마다)
		// RPL 472
	else if (o k l 의 param이 부족한 경우마다)
		// RPL 696
	else if (key 이미 설정됨)
		// RPL 467
	else
		// 동작 - timestamp
		// 해당 channel의 모든 유저에게 전송
	return (SUCCESS);
}

int	Irc::__not_a_command(Client *client, IRCMessage message)
{
	// RPL_421_err_unknowncommand
	return (SUCCESS);
}
