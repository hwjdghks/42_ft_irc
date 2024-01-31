#include "Client.hpp"
#include "Channel.hpp"

Channel::Channel(void) {
	option[TITLE] = false;
	option[OP] = false;
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
	this->option[OP] = ref.option[OP];
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
