#ifndef IRC_HPP
# define IRC_HPP

# include "def.hpp"
# include "Channel.hpp"
# include "Client.hpp"

class Irc
{
private:
	t_send_event				send_msg;
	std::string					password;
	std::vector<Client>			clients;
	std::vector<Channel>		channels;
	std::vector<std::string>	commands;
public:
	Irc();
	Irc(const Irc &ref);
	~Irc();

	Irc &operator=(const Irc &ref);

	void setPassword(std::string password);

	int createClient(int fd);
	t_send_event executeCommand(int fd, std::string recv_buffer);
	Client *searchClient(int fd);
	t_send_event ping(int fd);
	t_send_event quit(int fd, char *reason); // timeout, socket 문제
	t_send_event deleteClient(int fd);

private:
	int _register_executor(Client *client, IRCMessage recv_msg);
	int __register_user(Client *client, IRCMessage message);
	int __register_pass(Client *client, IRCMessage message);
	int __register_nick(Client *client, IRCMessage message);
	bool __isCommand(std::string cmd);
private:
	int _command_executor(Client *client, IRCMessage recv_msg);
	int __cmd_user(Client *client, IRCMessage message);
	int __cmd_pass(Client *client, IRCMessage message);
	int __cmd_nick(Client *client, IRCMessage message);
	int __cmd_ping(Client *client, IRCMessage message);
	int __cmd_pong(Client *client, IRCMessage message);
	int __cmd_list(Client *client, IRCMessage message);
	int __cmd_who(Client *client, IRCMessage message);
	int __cmd_quit(Client *client, IRCMessage message);
	int __cmd_privmsg(Client *client, IRCMessage message);
	int __cmd_join(Client *client, IRCMessage message);
	int __cmd_part(Client *client, IRCMessage message);
	int __cmd_topic(Client *client, IRCMessage message);
	int __cmd_kick(Client *client, IRCMessage message);
	int __cmd_invite(Client *client, IRCMessage message);
	int __cmd_mode(Client *client, IRCMessage message);
	int __not_a_command(Client *client, IRCMessage message);
	int ___isPassibleNickName();
	int ___isPassibleChannelName();
private: // irc_rpl.cpp
	std::string _001_rpl_welcome(std::string prefix, std::string clientnick, std::string user_prefix);
	std::string _321_rpl_liststart(std::string prefix, std::string clientnick);
	std::string _322_rpl_list(std::string prefix, std::string clientnick, std::string channelname, std::string usercnt, std::string topic);
	std::string _323_rpl_listend(std::string prefix, std::string clientnick);
	std::string _352_rpl_whoreply(std::string prefix, std::string clientnick, std::string channelname, std::string username, std::string op, std::string realname);
	std::string _315_rpl_endofwho(std::string prefix, std::string clientnick, std::string channelname);
	std::string _331_rpl_notopic(std::string prefix, std::string clientnick, std::string channelname);
	std::string _332_rpl_topic(std::string prefix, std::string clientnick, std::string channelname, std::string topic);
	std::string _341_rpl_inviting(std::string prefix, std::string clientnick, std::string channelname, std::string nick);
	std::string _324_rpl_channelmodeis(std::string prefix, std::string clientnick, std::string mode);
private: // irc_err.cpp
	std::string _421_err_unknowncommand(std::string prefix, std::string clientnick, std::string command);
	std::string _432_err_erroneusnickname(std::string prefix, std::string clientnick, std::string nick);
	std::string _433_err_nicknameinuse(std::string prefix, std::string clientnick, std::string nick);
	std::string _461_err_needmoreparams(std::string prefix, std::string clientnick, std::string command);
	std::string _462_err_alreadyregisterd(std::string prefix, std::string clientnick);
	std::string _451_err_notregistered(std::string prefix, std::string clientnick);
	std::string _403_err_nosuchchannel(std::string prefix, std::string clientnick, std::string channelname);
	std::string _405_err_toomanychannels(std::string prefix, std::string clientnick, std::string channelname);
	std::string _471_err_channelisfull(std::string prefix, std::string clientnick, std::string channelname);
	std::string _473_err_inviteonlychan(std::string prefix, std::string clientnick, std::string channelname);
	std::string _475_err_badchannelkey(std::string prefix, std::string clientnick, std::string channelname);
	std::string _442_err_notonchannel(std::string prefix, std::string clientnick, std::string channelname);
	std::string _401_err_nosuchnick(std::string prefix, std::string clientnick, std::string nick);
	std::string _443_err_useronchannel(std::string prefix, std::string clientnick, std::string channelname);
	std::string _482_err_chanoprivsneeded(std::string prefix, std::string clientnick, std::string channelname);
	std::string _472_err_unknownmode(std::string prefix, std::string clientnick, std::string chars);
	std::string _467_err_keyset(std::string prefix, std::string clientnick, std::string channelname);
};

#endif
