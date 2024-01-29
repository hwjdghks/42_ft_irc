#include "Irc.hpp"


std::string _str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

bool __isValidNick(const std::string &nick)
{
	return std::all_of(nick.begin(), nick.end(), ___isValidChar);
}

bool ___isValidChar(const char c) 
{
	return (std::isalnum(c) || c == '-' || c == '_') && std::isprint(c) && !std::isspace(c);
}


int Irc::__register_user(Client *client, IRCMessage message)
{
	if (message.parameters.size() >= 4)
	{
		if ( client->getUsername() != "")
		{
			this->send_msg.recv_work = true;
			this->send_msg.recv_time = false;
			this->send_msg.recv_close = false;
			this->send_msg.to_send = true;
			this->send_msg.fds.push_back(client->getFd());			
			_462_err_alreadyregisterd(SERVERURL, client->getNickname());
			return FAIL;
		}
		else
		{
			this->send_msg.recv_work = false;
			this->send_msg.recv_time = true;
			this->send_msg.recv_close = false;
			this->send_msg.to_send = false;		
			this->send_msg.fds.push_back(client->getFd());		
			client->setUsername(message.parameters[0]);
			return SUCCESS;
		}
	}
	else
	{
		this->send_msg.recv_work = true;
		this->send_msg.recv_time = false;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = true;		
		this->send_msg.fds.push_back(client->getFd());		
		_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command);
		return FAIL;
	}
}

int Irc::__register_pass(Client* client, IRCMessage message)
{
	if (message.parameters.size() == 0)
	{
		this->send_msg.recv_work = true;
		this->send_msg.recv_time = false;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = true;		
		this->send_msg.fds.push_back(client->getFd());		
		_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command);
		return FAIL;
    }
	else
	{
		this->send_msg.recv_work = false;
		this->send_msg.recv_time = true;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = false;		
		this->send_msg.fds.push_back(client->getFd());		
		client->setPassword(message.parameters[0]);
		return SUCCESS;
	}
}

int Irc::__register_nick(Client* client, IRCMessage message)
{
	if (message.parameters.size() == 0)
	{
		this->send_msg.recv_work = true;
		this->send_msg.recv_time = false;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = true;		
		this->send_msg.fds.push_back(client->getFd());		
		_461_err_needmoreparams(SERVERURL, client->getNickname(), message.command);
		return FAIL;
	}
	else if (isDupNick(client, message.parameters[0])) // nick 중복 여부 확인
	{
		this->send_msg.recv_work = true;
		this->send_msg.recv_time = false;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = true;		
		this->send_msg.fds.push_back(client->getFd());		
		_433_err_nicknameinuse(SERVERURL, client->getNickname(), message.parameters[0]);
		return FAIL;
	}
	else if (message.parameters[0].size() >= 9 && message.parameters[0].size() <= 30 && __isValidNick(message.parameters[0]))
	{
		this->send_msg.recv_work = true;
		this->send_msg.recv_time = false;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = true;
		this->send_msg.fds.push_back(client->getFd());		
        _432_err_erroneusnickname(SERVERURL, client->getNickname(), message.parameters[0]);
		return FAIL;
	}
	else
	{
		this->send_msg.recv_work = false;
		this->send_msg.recv_time = true;
		this->send_msg.recv_close = false;
		this->send_msg.to_send = false;
		this->send_msg.fds.push_back(client->getFd());		
		client->setNickname(message.parameters[0]);
		return SUCCESS;
	}
}


IRCMessage Irc::parseMessage(const char message[])
{
	IRCMessage ircMessage;
	
	std::istringstream iss(message);
	std::string token;
	size_t found;

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