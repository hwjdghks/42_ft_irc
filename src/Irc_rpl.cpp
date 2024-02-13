#include "Irc.hpp"

std::string Irc::_001_rpl_welcome(std::string prefix, std::string clientnick, std::string user_prefix)
{
	std::string str = prefix + " 001 " + clientnick + " :" + user_prefix + "\r\n";
	return (str);
}

std::string Irc::_321_rpl_liststart(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 321 " + clientnick + " Channel :Users  Name" + "\r\n";
	return (str);
}

std::string Irc::_322_rpl_list(std::string prefix, std::string clientnick, std::string channelname, std::string usercnt, std::string topic)
{
	std::string str = prefix + " 322 " + clientnick + " " + channelname + " " + usercnt + " :" + topic + "\r\n";
	return (str);
}

std::string Irc::_323_rpl_listend(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 323 " + clientnick + " :End of /LIST" + "\r\n";
	return (str);
}

std::string Irc::_352_rpl_whoreply(std::string prefix, std::string clientnick, std::string channelname, std::string username, std::string nickname, std::string op, std::string realname)
{
	std::string str = prefix + " 352 " + clientnick + " " + channelname + " " + username + " " + HOST + " " + SERVERNAME + " " + nickname + " H" + op + " :0";
	str += " " + realname + "\r\n";
	return (str);
}

std::string Irc::_315_rpl_endofwho(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 315 " + clientnick + " " + channelname + " :End of /WHO list." + "\r\n";
	return (str);
}

std::string Irc::_331_rpl_notopic(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 331 " + clientnick + " " + channelname + " :No topic is set." + "\r\n";
	return (str);
}

std::string Irc::_332_rpl_topic(std::string prefix, std::string clientnick, std::string channelname, std::string topic)
{
	std::string str = prefix + " 332 " + clientnick + " " + channelname + " :" + topic + "\r\n";
	return (str);
}

std::string Irc::_341_rpl_inviting(std::string prefix, std::string clientnick, std::string channelname, std::string nick)
{
	std::string str = prefix + " 341 " + clientnick + " " + channelname + " " + nick + "\r\n";
	return (str);
}

std::string Irc::_324_rpl_channelmodeis(std::string prefix, std::string clientnick, std::string channelname, std::string mode, std::string mode_params)
{
	std::string str = prefix + " 324 " + clientnick + " " + channelname  + " " + mode  + " " + mode_params + "\r\n";
	return (str);
}

std::string Irc::_332_rpl_(std::string prefix, std::string clientnick, std::string channelname, std::string topic)
{
	std::string str = prefix + " 332 " + clientnick + " " + channelname + topic + "\r\n";
	return (str);
}

std::string Irc::_353_rpl_(std::string prefix, std::string clientnick, std::string channelname, std::string chan_users)
{
	std::string str = prefix + " 353 " + clientnick + " = " + channelname + chan_users + "\r\n";
	return (str);
}
std::string Irc::_366_rpl_(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 366 " + clientnick + " " + channelname + " :End of /NAMES list." +"\r\n";
	return (str);
}
