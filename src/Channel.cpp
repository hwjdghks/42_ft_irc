#include "Client.hpp"
#include "Channel.hpp"
#include <sstream>

Channel::Channel(void) {
	option[TITLE] = false;
	option[KEY] = false;
	option[LIMIT] = false;
	option[INVITE] = false;
	bot.setHostname("localhost");
	bot.setNickname(BOTNAME);
	bot.setUsername("ft_irc");
	bot.setRealname("RoBoT");
	bot.setBot(true);
	bot.setLife(false);
	users.push_back(&bot);
}

Channel::Channel(const Channel &ref)
{
	*this = ref;
}

Channel::~Channel(void) {}

Channel &Channel::operator=(const Channel &ref)
{
	if (this == &ref)
		return *this;
	this->option[TITLE] = ref.option[TITLE];
	this->option[KEY] = ref.option[KEY];
	this->option[LIMIT] = ref.option[LIMIT];
	this->option[INVITE] = ref.option[INVITE];
	this->limit = ref.limit;
	this->password = ref.password;
	this->topic = ref.topic;
	this->name = ref.name;
	this->operators = ref.operators;
	this->users = ref.users;
	this->invited = ref.invited;
	return *this;
}

void Channel::setOptionTitle(bool title)
{
	this->option[TITLE] = title;
}

void Channel::setOptionkey(bool key)
{
	this->option[KEY] = key;
}

void Channel::setOptionLimit(bool limit)
{
	this->option[LIMIT] = limit;
}

void Channel::setOptionInvite(bool invite)
{
	this->option[INVITE] = invite;
}

bool Channel::getOptionTitle(void)
{
	return (option[TITLE]);
}

bool Channel::getOptionkey(void)
{
	return (option[KEY]);
}

bool Channel::getOptionLimit(void)
{
	return (option[LIMIT]);
}

bool Channel::getOptionInvite(void)
{
	return (option[INVITE]);
}

void Channel::delPassword()
{
	password.clear();
}

std::vector<Client *> &Channel::getOperators(void)
{
	return this->operators;
}

std::vector<Client *> &Channel::getUsers(void)
{
	return this->users;
}

void Channel::setPassword(std::string password)
{
	this->password = password;
}

void Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

void Channel::setName(std::string name)
{
	this->name = name;
}

std::string Channel::getPassword(void)
{
	return password;
}

std::string Channel::getTopic(void)
{
	return topic;
}

std::string Channel::getName(void)
{
	return name;
}

void Channel::setLimit(size_t limit)
{
	this->limit = limit;
}

size_t Channel::getLimit(void)
{
	return(limit);
}

Client *Channel::getBot()
{
	return (&bot);
}

bool Channel::isKey(std::string key)
{
	if (password == key)
		return (true);
	return (false);
}

bool Channel::isFull()
{
	if (users.size() >= limit)
		return (true);
	return (false);
}

// :penguin.omega.example.org 324 jijeong #asdf +klnt 123243546 :10
// :penguin.omega.example.org 324 kiryud #asdf +klnt <key> :10
std::string Channel::getMode()
{
	std::string str;
	str = "+";
	if (option[TITLE])
		str += "t";
	if (option[KEY])
		str += "k";
	if (option[LIMIT])
		str += "l";
	if (option[INVITE])
		str += "i";
	return (str);
}

std::string Channel::getModeParam(std::string nickname)
{
	std::string str;

	if (option[KEY])
	{
		if (isUser(nickname))
			str += password;
		else
			str += "<KEY>";
		str += " ";
	}
	str += ":";
	if (option[LIMIT])
	{
		std::stringstream ss;
		ss << limit;
		std::string limit_num = ss.str();
		str += limit_num;
	}return (str);
}


bool Channel::addInvite(Client *client)
{
	if (isInvite(client->getNickname()))
		return false;
	invited.push_back(client);
	return true;
}

bool Channel::isInvite(const std::string &nickname)
{
	for (std::vector<Client *>::const_iterator it = invited.begin(); it != invited.end(); it++)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

void Channel::delInvite(const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
		{
			invited.erase(it);
			break ;
		}
	}
}

bool Channel::addOperator(Client *client)
{
	if (isOperator(client->getNickname()))
		return false;
	operators.push_back(client);
	return true;
}

bool Channel::isOperator(const std::string &nickname) const
{
	for (std::vector<Client *>::const_iterator it = operators.begin(); it != operators.end(); it++)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

void Channel::delOperator(const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
		{
			operators.erase(it);
			break ;
		}
	}
}

bool Channel::addUser(Client *client)
{
	if (isUser(client->getNickname()))
		return false;
	users.push_back(client);
	return true;
}

bool Channel::isUser(const std::string &nickname) const
{
	for (std::vector<Client *>::const_iterator it = users.begin(); it != users.end(); it++)
		if ((*it)->getNickname() == nickname)
			return true;
	return false;
}

void Channel::delUser(const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); it++)
	{
		if ((*it)->getNickname() == nickname)
		{
			users.erase(it);
			break ;
		}
	}
}
