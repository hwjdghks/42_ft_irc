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

int Irc::createClient(int fd)
{
	if (!(clients.size() < MAX_CLIENT))
		return (-1);
	// client vector에 client 추가하기
	Client new_client;
	
	new_client.setFd(fd);
	this->clients.push_back(new_client);
}

t_send_event Irc::executeCommand(int fd, std::string recv_buffer)
{
	// 해당 fd의 클라이언트 찾아오기
	Client *client;
	// 메세지 리드버퍼에 저장
	// 리드버퍼 해석 <개행 찾기>
	// 없으면 종료
	// 리시브 버퍼 해석
	IRCMessage recv_msg;
	// 명령어에 따라 동작하기
	if (!client.isRegistered())
		_register_executor();
	_command_executor(recv_msg);
}

int	Irc::_register_executor(IRCMessage recv_msg)
{
	if (recv_msg.command == "USER")
		__register_user(recv_msg);
	else if (recv_msg.command == "PASS")
		__register_pass(recv_msg);
	else if (recv_msg.command == "NICK")
		__register_nick(recv_msg);
	else if (__isCommand(recv_msg.command))
		// RPL
	return (SUCCESS);
}

int	Irc::_command_executor(IRCMessage recv_msg)
{
	if (recv_msg.command == "USER")
		__cmd_user(recv_msg);
	else if (recv_msg.command == "PASS")
		__cmd_pass(recv_msg);
	else if (recv_msg.command == "NICK")
		__cmd_nick(recv_msg);
	else if (recv_msg.command == "PING")
		__cmd_ping(recv_msg);
	else if (recv_msg.command == "PONG")
		__cmd_pong(recv_msg);
	else if (recv_msg.command == "LIST")
		__cmd_list(recv_msg);
	else if (recv_msg.command == "WHO")
		__cmd_who(recv_msg);
	else if (recv_msg.command == "QUIT")
		__cmd_quit(recv_msg);
	else if (recv_msg.command == "PRIVMSG")
		__cmd_privmsg(recv_msg);
	else if (recv_msg.command == "JOIN")
		__cmd_join(recv_msg);
	else if (recv_msg.command == "PART")
		__cmd_part(recv_msg);
	else if (recv_msg.command == "TOPIC")
		__cmd_topic(recv_msg);
	else if (recv_msg.command == "KICK")
		__cmd_kick(recv_msg);
	else if (recv_msg.command == "INVITE")
		__cmd_invite(recv_msg);
	else if (recv_msg.command == "MODE")
		__cmd_mode(recv_msg);
	else
		__not_a_command(recv_msg);
	return (SUCCESS);
}

int	Irc::__cmd_ping(IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL 461
	else if (message.parameters.size() == 1)
		// :prifix PONG server :message.parameters
	else
		// :prifix PONG message.parameters[1] :message.parameters[0]
		// timestamp
	return (SUCCESS);
}

int Irc::__cmd_pong(IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL 461
	else
		//timestamp
	return (SUCCESS);
}

int Irc::__cmd_list(IRCMessage message)
{
	// RPL 321
	for (int i = 0; i < channels.size() ; i++)
		// RPL 322
	// RPL 323
	return (SUCCESS);
}

int Irc::__cmd_who(IRCMessage message)
{
	if (message.parameters.size() == 0)
		// RPL 461
	else if (_is_nick_exist(message.parameters[0]))
		// RPL 352
	else if (_is_channel_exist(message.parameters[0]))
		for ()
			// RPL 352
	// RPL 315
	// timestamp
	return (SUCCESS);
}

int Irc::__cmd_quit(IRCMessage message)
{
	if (message.parameters.size() == 0)
		// quit
	else
		// user의 모든 chan에 quit 
	return (SUCCESS);
}

int	Irc::__not_a_command(IRCMessage message)
{
	return (SUCCESS);
}