#include "Irc.hpp"

std::string _str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

bool ___isValidChar(const char c) 
{
	return (std::isalnum(c) || c == '-' || c == '_') && std::isprint(c) && !std::isspace(c);
}

bool Irc::__isValidChannelName(const std::string &chName)
{
	std::string tmpStr;
	if (chName.empty())
		return false;
	if (chName.length() > 32)
		return false;
	if (chName[0] != '&' && chName[0] != '#')
		return false;
	tmpStr = chName.substr(1);
	for (size_t i = 0; i < tmpStr.length(); i++)
	{
		if (!___isValidChar(tmpStr[i]))
			return false;
	}
  	return true;
}

bool Irc::__isValidNick(const std::string &nick)
{
	if (nick.length() > 9)
		return false;
	for (size_t i = 0; i < nick.length(); i++)
	{
		if (!___isValidChar(nick[i]))
			return false;
	}
	return true;
}

bool Irc::_isNickInUse(Client* cur_client, std::string to_be_nick)
{
	for (std::list<Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (cur_client->getFd() == it->getFd())
			continue;
		if (it->getNickname() == to_be_nick)
			return true;
	}
	return false;
}

int Irc::__register_user(Client *client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	if (message.parameters.size() >= 4)
	{
		if ( client->getUsername() != "")
		{
			_setSendEvent(false, false, false, true, fds);
			client->addWrite_buffer(_462_err_alreadyregisterd(SERVERURL, client->getNickname()));
			return FAIL;
		}
		else
		{
			_setSendEvent(false, false, false, false, fds);
			client->setUsername(message.parameters[0]);
			client->setRealname(message.parameters[3]);
			client->setRegi(USER, true);
			return SUCCESS;
		}
	}
	else
	{
		_setSendEvent(false, false, false, true, fds);		
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
	}
}

int Irc::__register_pass(Client* client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	if (message.parameters.size() == 0)
	{
		_setSendEvent(false, false, false, true, fds);
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
    }
	else
	{
		_setSendEvent(false, false, false, false, fds);
		client->setPassword(message.parameters[0]);
		client->setRegi(PASS, true);
		return SUCCESS;
	}
}

int Irc::__register_nick(Client* client, IRCMessage message)
{
	std::vector<int> fds;
	if (client->isAlive())
		fds.push_back(client->getFd());
	if (message.parameters.size() == 0)
	{
		_setSendEvent(false, false, false, true, fds);
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
	}
	else
	{
		client->setRegi(NICK, true); 
		if (_isNickInUse(client, message.parameters[0])) // nick 중복 여부 확인
		{
			_setSendEvent(false, false, false, true, fds);
			client->addWrite_buffer(_433_err_nicknameinuse(SERVERURL, client->getNickname(), message.parameters[0]));
			return FAIL;
		}
		else if (!__isValidNick(message.parameters[0]))
		{
			_setSendEvent(false, false, false, true, fds);
			client->addWrite_buffer(_432_err_erroneusnickname(SERVERURL, client->getNickname(), message.parameters[0]));
			return FAIL;
		}
		else
		{
			_setSendEvent(false, false, false, false, fds);
			client->setNickname(message.parameters[0]);
			return SUCCESS;
		}
	}
}

IRCMessage Irc::parseMessage(std::string message)
{
    IRCMessage ircMessage;
    std::istringstream iss(message);
    std::string token;
    // Extracting prefix
    if (message[0] == ':')
    {
        iss >> token;
        ircMessage.prefix = token.substr(1);
    }
    // Extracting command
    iss >> token;
    ircMessage.command = _str_toupper(token);
    // Extracting parameters
    while (iss >> token)
    {
        if (token.find(":") != std::string::npos)
        {
            iss.seekg((token.length() - 1) * -1, iss.cur);
            std::getline(iss, token);
            ircMessage.parameters.push_back(token);
			return ircMessage;
        }
        else
        {
            ircMessage.parameters.push_back(token);
        }
    }
    return ircMessage;
}

bool Irc::isExistingClient(std::string clName)
{
	std::list<Client>::iterator it;
	for(it = clients.begin(); it != clients.end(); it++)
	{
		if (it->getNickname() == clName)
			return true;
	}
	return false;
}

bool Irc::isExistingChannel(std::string chName)
{
	std::list<Channel>::iterator it;
	for(it = channels.begin(); it != channels.end(); it++)
	{
		if (it->getName() == chName)
			return true;
	}
	return false;
}


std::vector<std::string> Irc::__getTargets(const std::string &stargets)
{
	std::vector<std::string>targets;
	std::stringstream ss(stargets);
	std::string starget;

	if (stargets.size() > 0 && std::string::npos == stargets.find(','))
		targets.push_back(stargets);
	else
	{
		while (std::getline(ss, starget, ',')) 
		{
			targets.push_back(starget);
		}
	}
	return targets;
}

bool Irc::___check_slang(const std::string &msg)
{
    std::string slangs[] = BADWORDS;
    std::string tmpMsg = _str_toupper(msg);
    for(size_t i = 0; i < sizeof(slangs) / sizeof(slangs[0]); i++)
    {
        if (tmpMsg.find(_str_toupper(slangs[i])) != std::string::npos)
            return true;
    }
    return false;
}