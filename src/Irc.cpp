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

int Irc::createClient(int fd)
{
	if (!(clients.size() < MAX_CLIENT))
		return (FAIL);
	// client vector에 client 추가하기
	// 기존 서버에 존재하는 코드 그대로 가져옴
	Client new_client;
	
	new_client.setFd(fd);
	this->clients.push_back(new_client);
	return (SUCCESS);
}

t_send_event Irc::executeCommand(int fd, std::string recv_buffer)
{
	// 해당 fd의 클라이언트 찾아오기
	Client *client = searchClient(fd);
	// 메세지 리드버퍼에 저장
	// 리드버퍼 해석 <개행 찾기>
	// 없으면 종료
	// 리시브 버퍼 해석
	IRCMessage recv_msg;
	// 명령어에 따라 동작하기
	if (!client->isRegistered())
		_register_executor(client, recv_msg);
	_command_executor(client, recv_msg);
}

Client *searchClient(int fd)
{
	// client vector를 순회하여 fd에 해당하는 클라이언트 가져오기
	return ();
}

t_send_event ping(int fd)
{
	// t_send_event에 fd를 넣는다
	// 해당 fd의 클라이언트를 찾는다
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	return ();
}

t_send_event quit(int fd)
{
	// 해당 fd의 클라이언트를 찾는다
	// 해당 클라이언트가 속한 채널의 유저들을 찾는다
	// 해당 유저들의 fd를 t_send_event에 넣는다
	// write buffer에 PING 메세지를 넣고 t_send_event 반환
	return ();
}

t_send_event deleteClient(int fd)
{
	// 해당 fd의 클라이언트를 찾는다
	// 해당 클라이언트가 가입된 채널을 찾아 제거한다
	// 클라이언트 벡터에서 해당 클라이언트를 제거한다.
	return ();
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
	// timestamp
	return (SUCCESS);
}

int Irc::__cmd_quit(Client *client, IRCMessage message)
{
	if (message.parameters.size() == 0)
		// 동작 : 메세지 공란
	else
		// 동작 : 메세지 같이 출력
	return (SUCCESS);
}

int Irc::__cmd_privmsg(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_join(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_part(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_topic(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_kick(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_invite(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int Irc::__cmd_mode(Client *client, IRCMessage message)
{
	return (SUCCESS);
}
int	Irc::__not_a_command(Client *client, IRCMessage message)
{
	// RPL_421_err_unknowncommand
	return (SUCCESS);
}