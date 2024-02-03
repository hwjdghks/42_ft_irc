#include "Client.hpp"
#include "Channel.hpp"

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


std::vector<Client *> &Channel::getOperators(void)
{
	return this->operators;
}

std::vector<Client *> &Channel::getUsers(void)
{
	return this->users;
}

bool Channel::addOperator(Client &client)
{
	if (isOperator(client.getNickname()))
		return false;
	operators.push_back(&client);
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

bool Channel::addUser(Client &client)
{
	if (isUser(client.getNickname()))
		return false;
	users.push_back(&client);
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
