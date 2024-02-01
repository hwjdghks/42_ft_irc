#include "Irc.hpp"

std::string Irc::_001_rpl_welcome(std::string prefix, std::string clientnick, std::string user_prefix)
{
	std::string str = prefix + " 001 " + clientnick + " :" + user_prefix + "\n";
	return (str);
}

std::string Irc::_321_rpl_liststart(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 321 " + clientnick + " Channel :Users  Name" + "\n";
	return (str);
}

std::string Irc::_322_rpl_list(std::string prefix, std::string clientnick, std::string channelname, std::string usercnt, std::string topic)
{
	std::string str = prefix + " 322 " + clientnick + " " + channelname + " " + usercnt + ": " + topic + "\n";
	return (str);
}

std::string Irc::_323_rpl_listend(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 323 " + clientnick + " :End of /LIST" + "\n";
	return (str);
}

std::string Irc::_352_rpl_whoreply(std::string prefix, std::string clientnick, std::string channelname, std::string username, std::string op, std::string realname)
{
	std::string str = prefix + " 352 " + clientnick + " " + channelname + " " + username + " " + HOST + " " + SERVERURL + " H" + op + " :0 ";
	str + " " + username + "\n";
	return (str);
}

std::string Irc::_315_rpl_endofwho(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 315 " + clientnick + " " + channelname + " :End of /WHO list." + "\n";
	return (str);
}

std::string Irc::_331_rpl_notopic(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 331 " + clientnick + " " + channelname + " :No topic is set." + "\n";
	return (str);
}

std::string Irc::_332_rpl_topic(std::string prefix, std::string clientnick, std::string channelname, std::string topic)
{
	std::string str = prefix + " 332 " + clientnick + " " + channelname + " :" + topic + "\n";
	return (str);
}

std::string Irc::_341_rpl_inviting(std::string prefix, std::string clientnick, std::string channelname, std::string nick)
{
	std::string str = prefix + " 341 " + clientnick + " " + channelname + " " + nick + "\n";
	return (str);
}

std::string Irc::_324_rpl_channelmodeis(std::string prefix, std::string clientnick, std::string mode)
{
	std::string str = prefix + " 324 " + clientnick + " " + mode + "\n";
	return (str);
}
