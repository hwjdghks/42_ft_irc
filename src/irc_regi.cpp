#include "Irc.hpp"

int IRC::__register_user(Client* client, IRCMessage message)
{
	if (message.parameters.size() >= 4)
	{
		if ( client->getUsername() != "")
		{
			sendMSG(client, _462_alreadyregisterd(SERVERURL, client->getNickname()));
            return FAIL;
		}
		else
		{
			client->setUsername(message.parameters[0]);
            return SUCCESS;
		}
	}
	else
	{
		sendMSG(client, _461_error_needmoreparams(SERVERURL, client->getNickname()), "USER");
        return FAIL;
	}
}

int IRC::__register_pass(Client* client, IRCMessage message)
{
	if (message.parameters.size() == 0)
	{
		sendMSG(client, _461_err_needmoreparams(SERVERURL, client->getNickname()), "PASS");
        return FAIL;
    }
	else
	{
		client->setPassword(message.parameters[0]);
        return SUCCESS;
	}
}

int IRC::__register_nick(Client* client, IRCMessage message)
{
	if (message.parameters.size() == 0)
	{
		sendMSG(client, _461_err_needmoreparams(SERVERURL, client->getNickname()), "NICK");
        return FAIL;
	}
	else if (isDupNick(client, parameters[0])) // nick 중복 여부 확인
	{
		sendMSG(client, _433_err_nicknameinuse(SERVERURL, client->getNickname(), message.parameters[0]);
        return FAIL;
	}
	else if (parameters[0].size() >= 9 && parameters[0].size() <= 30 && __isValidNick(parameters[0]))
	{
        sendMSG(client, _432_errorneousnickname(SERVERURL, client->getNickname()), message.parameters[0]);
        return FAIL;
	}
	else
	{
		client->setNickname(parameters[0]);
        return SUCCESS;
	}
}

bool IRC::__isValidNick(const std::string &nick)
{
	return std::all_of(str.begin(), str.end(), ___isValidChar);
}

bool IRC::___isValidChar(const char c) 
{
    return (std::isalnum(c) || c == '-' || c == '_') && std::isprint(c) && !std::isspace(c);
}