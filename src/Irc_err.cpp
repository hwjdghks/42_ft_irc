#include "Irc.hpp"

std::string Irc::_401_err_nosuchnick(std::string prefix, std::string clientnick, std::string nick)
{
	std::string str = prefix + " 401 " + clientnick  + " " + nick + " :No such nick/channel" + "\r\n";
	return (str);
}

std::string Irc::_403_err_nosuchchannel(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 403 " + clientnick  + " " + channelname + " :No such channel" + "\r\n";
	return (str);
}

std::string Irc::_405_err_toomanychannels(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 405 " + clientnick  + " " + channelname + " :You have joined too many channels" + "\r\n";
	return (str);
}

std::string Irc::_421_err_unknowncommand(std::string prefix, std::string clientnick, std::string command)
{
	std::string str = prefix + " 421 " + clientnick  + " " + command + " :Unknown command" + "\r\n";
	return (str);
}

std::string Irc::_432_err_erroneusnickname(std::string prefix, std::string clientnick, std::string nick)
{
	std::string str = prefix + " 432 " + clientnick  + " " + nick + " :Erroneus nickname" + "\r\n";
	return (str);
}

std::string Irc::_433_err_nicknameinuse(std::string prefix, std::string clientnick, std::string nick)
{
	std::string str = prefix + " 433 " + clientnick  + " " + nick + " :Nickname is already in use" + "\r\n";
	return (str);
}

std::string Irc::_461_err_needmoreparams(std::string prefix, std::string clientnick, std::string command)
{
	std::string str = prefix + " 461 " + clientnick  + " " + command + " :Not enough parameters" + "\r\n";
	return (str);
}

std::string Irc::_462_err_alreadyregisterd(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 462 " + clientnick  + " :You may not reregister" + "\r\n";
	return (str);
}

std::string Irc::_451_err_notregistered(std::string prefix, std::string clientnick)
{
	std::string str = prefix + " 451 " + clientnick + " :You have not registered" + "\r\n";
	return (str);
}

std::string Irc::_471_err_channelisfull(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 471 " + clientnick + " " + channelname + " :Cannot join channel (+l)" + "\r\n";
	return (str);
}

std::string Irc::_472_err_unknownmode(std::string prefix, std::string clientnick, std::string chars)
{
	std::string str = prefix + " 472 " + clientnick + " " + chars + " :is unknown mode char to me" + "\r\n";
	return (str);
}

std::string Irc::_473_err_inviteonlychan(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 473 " + clientnick + " " + channelname + " :Cannot join channel (+i)" + "\r\n";
	return (str);
}

std::string Irc::_475_err_badchannelkey(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 475 " + clientnick + " " + channelname + " :Cannot join channel (+k)" + "\r\n";
	return (str);
}

std::string Irc::_442_err_notonchannel(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 442 " + clientnick + " " + channelname + " :You're not on that channel" + "\r\n";
	return (str);
}

std::string Irc::_443_err_useronchannel(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 443 " + clientnick + " " + clientnick + " " + channelname + " :is already on channel" + "\r\n";
	return (str);
}

std::string Irc::_482_err_chanoprivsneeded(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 482 " + clientnick + " " + channelname + " :You're not channel operator" + "\r\n";
	return (str);
}

std::string Irc::_467_err_keyset(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 467 " + clientnick + " " + channelname + " :Channel key already set" + "\r\n";
	return (str);
}

std::string Irc::_476_err_badchanmask(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 476 " + clientnick + " " + channelname + " :Bad Channel Mask" + "\r\n";
	return (str);
}

std::string Irc::_696_err_invalidmodeparam(std::string prefix, std::string clientnick, std::string channelname)
{
	std::string str = prefix + " 467 " + clientnick + " " + channelname + " :need more param" + "\r\n";
	return (str);
}
