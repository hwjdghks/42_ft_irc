#include "Server.hpp"

std::string Server::str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

IRCMessage Server::parseMessage(const char message[])
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
	ircMessage.command = str_toupper(token);

	// Extracting parameters
	while (iss >> token) 
	{
		ircMessage.parameters.push_back(token);
	}

	return ircMessage;
}

bool Server::isValidChar(const char c) 
{
    return (std::isalnum(c) || c == '-' || c == '_') && std::isprint(c) && !std::isspace(c);
}

bool Server::isValidNick(const std::string &str) 
{
    return std::all_of(str.begin(), str.end(), isValidChar);
}

//client와 waiting_client들 중 자신을 제외한 instance와 nick 중복 여부 확인
bool Server::isDupNick(const std::string &nick, const int &fd)
{
	std::vector<Client>::iterator it;

	//check resigstered clients
	for(it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->getFd() != fd && it->getNickname() == nick)
			return true;
	//check waiting clients	
	for(it = this->waiting_clients.begin(); it != this->waiting_clients.end(); it++)
		if (it->getFd() != fd && it->getNickname() == nick)
			return true;

	return false
}

void Server::handleMessage(const IRCMessage &message, const int &fd)
{
	//해당 client instance search
    int loc;
	std::vector<Client>::iterator current_client = getCurrentClient(fd, &loc);

	if (loc != 1) //CASE1 <-  등록 전 user
	{	
		if (message.command == "USER")
		{
			if (message.parameters.size() >= 4)
			{
				if ( current_client->getUsername() != "")
				{
					//RPL reregister;
				}
				else
				{
					current_client->setUsername(message.parameters[0]);
				}
			}
			else
			{
				//RPL 461 :Not enought parameters;
			}
		}
		else if (message.command == "PASS")
		{
			if (message.parameters.size() == 0)
			{
				//RPL 461 :Not enought parameters;
			}
			else
			{
				current_client->setPassword(message.parameters[0]);
			}
		}
		if (message.command == "NICK")
		{
			if (message.parameters.size() == 0)
			{
				//RPL 461 :Not enought parameters;
			}
			else if (isDupNick(message.parameters[0], fd)) // nick 중복 여부 확인
			{
				//RPL 433 :Nickname is already in use;
			}
			else if (message.parameters[0].size() >= 9 && message.parameters[0].size() <= 30 && isValidNick(message.parameters[0]))
			{
				//RPL 432 :Erroneous Nickname;
			}
			else
			{
				current_client->setNickname(message.parameters[0]);
			}
		}
		else if (message.command가 가용 command list상에 있으면)
		{
			//RPL :Not registered user yet
		}
	}
}
