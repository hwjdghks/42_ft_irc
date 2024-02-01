#include "Server.hpp"
#include "Client.hpp"

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
bool Server::isDupNick(Client* cur_client, const std::string &nick)
{
	std::vector<Client>::iterator it;

	//check resigstered clients
	for(it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->getFd() != cur_client->getFd() && it->getNickname() == nick)
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
			handleUSERcmd(&*current_client, message.parameters);
		else if (message.command == "PASS")
			handlePASScmd(&*current_client, message.parameters);
		if (message.command == "NICK")
			handleNICKcmd(&*current_client, message.parameters);
		else if (message.command가 가용 command list상에 있으면)
		{
			//451 :You have not registered
		}
	}
}

void Server::handleUSERcmd(Client* client, const std::vector<std::string>& parameters)
{
	struct IRCMessage send_message;

	if (parameters.size() >= 4)
	{
		if ( client->getUsername() != "")
		{
			send_message.prefix = '\0';
			send_message.command = "462";
			send_message.parameters[0] = ":You may not reregister";
			sendMSG(client, send_message);
		}
		else
		{
			client->setUsername(parameters[0]);
		}
	}
	else
	{
		send_message.prefix = '\0';
		send_message.command = "461";
		send_message.parameters[0] = "USER : Not enough parameters";
		sendMSG(client, send_message);
	}
}

void Server::handlePASScmd(Client* client, const std::vector<std::string>& parameters)
{
	struct IRCMessage send_message;
	if (parameters.size() == 0)
	{
		send_message.prefix = '\0';
		send_message.command = "461";
		send_message.parameters[0] = "USER : Not enough parameters";
		sendMSG(client, send_message);
	}
	else
	{
		client->setPassword(parameters[0]);
	}
}

void Server::handleNICKcmd(Client* client, const std::vector<std::string>& parameters)
{
	struct IRCMessage send_message;
	if (parameters.size() == 0)
	{
		//RPL 461 :Not enought parameters;
		send_message.prefix = '\0';
		send_message.command = "461";
		send_message.parameters[0] = "NICK : Not enough parameters";
		sendMSG(client, send_message);

	}
	else if (isDupNick(client, parameters[0])) // nick 중복 여부 확인
	{
		//RPL 433 :Nickname is already in use;
		send_message.prefix = '\0';
		send_message.command = "433";
		send_message.parameters[0] = parameters[0] + " : Nickname is already in use";
		sendMSG(client, send_message);

	}
	else if (parameters[0].size() >= 9 && parameters[0].size() <= 30 && isValidNick(parameters[0]))
	{
		send_message.prefix = '\0';
		send_message.command = "432";
		send_message.parameters[0] = parameters[0] + " : Erroneus nickname";
		sendMSG(client, send_message);
	}
	else
	{
		client->setNickname(parameters[0]);
	}
}





void Server::sendMSG(Client* receiver, struct IRCMessage send_message)
{
	;
}

void Server::sendMSG(Channel* receive_channel, struct IRCMessage send_message)
{
	//receive_channel->broadCast(send_message);
	;
}