#include "Irc.hpp"
#include <sstream>


std::string _str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

bool ___isValidChar(const char c) 
{
	return (std::isalnum(c) || c == '-' || c == '_') && std::isprint(c) && !std::isspace(c);
}

bool __isValidChannelName(const std::string &chName)
{
	std::string tmpStr;
	if (chName.empty())
		return false;
	if (chName.length() > 32)
		return false;
	if (chName[0] != '&' && chName[0] != '#')
		return false;
	tmpStr = chName.substr(1);
	if (!std::all_of(tmpStr.begin(), tmpStr.end(), ___isValidChar))
	 	return false;

	return true;
}

bool __isValidNick(const std::string &nick)
{
	if (nick.length() > 9)
		return false;
	if (!std::all_of(nick.begin(), nick.end(), ___isValidChar))
		return false;

	return true;
}

bool Irc::_isNickInUse(Client* cur_client, std::string to_be_nick)
{
	for (std::vector<Client>::iterator it = clients.begin(); it < clients.end(); it++)
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
	std::vector<int> fds(client->getFd());
	if (message.parameters.size() >= 4)
	{
		if ( client->getUsername() != "")
		{
			_setSendEvent(true, false, false, true, fds);
			client->addWrite_buffer(_462_err_alreadyregisterd(SERVERURL, client->getNickname()));
			return FAIL;
		}
		else
		{
			_setSendEvent(false, false, false, false, fds);
			client->setUsername(message.parameters[0]);
			return SUCCESS;
		}
	}
	else
	{
		_setSendEvent(true, false, false, true, fds);		
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
	}
}

int Irc::__register_pass(Client* client, IRCMessage message)
{
	std::vector<int> fds(client->getFd());
	if (message.parameters.size() == 0)
	{
		_setSendEvent(true, false, false, true, fds);
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
    }
	else
	{
		_setSendEvent(false, false, false, false, fds);
		client->setPassword(message.parameters[0]);
		return SUCCESS;
	}
}

int Irc::__register_nick(Client* client, IRCMessage message)
{
	std::vector<int> fds(client->getFd());	
	if (message.parameters.size() == 0)
	{
		_setSendEvent(true, false, false, true, fds);
		client->addWrite_buffer(_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command));
		return FAIL;
	}
	else if (_isNickInUse(client, message.parameters[0])) // nick 중복 여부 확인
	{
		_setSendEvent(true, false, false, true, fds);
		client->addWrite_buffer(_433_err_nicknameinuse(SERVERURL, client->getNickname(), message.parameters[0]));
		return FAIL;
	}
	else if (!__isValidNick(message.parameters[0]))
	{
		_setSendEvent(true, false, false, true, fds);		
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


IRCMessage Irc::parseMessage(const char message[])
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
	if (iss >> token)
	{
		if (token.find(":") != std::string::npos)
		{
			iss.seekg((token.length() - 1) * -1, iss.cur);
			std::getline(iss, token);
			ircMessage.parameters.push_back(token);
		}
		else
		{
			ircMessage.parameters.push_back(token);
			while (iss >> token) 
				ircMessage.parameters.push_back(token);
		}
	}

	return ircMessage;
}

bool Irc::isExistingChannel(std::string chName)
{
	std::vector<Channel>::iterator it;
	for(it = channels.begin(); it < channels.end(); it++)
	{
		if (it->getName() == chName)
			return true;
	}
	return false;
}

