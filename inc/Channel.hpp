#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "def.hpp"

class Client;

class Channel
{
private:
	bool						option[5];
	int							limit;
	std::string					password;
	std::string					topic;
	std::string					name;
	std::vector<Client *>		operators;
	std::vector<Client *>		users;
	std::vector<std::string>	invited;
public:
	Channel();
	Channel(const Channel &ref);
	~Channel();

	Channel &operator=(const Channel &ref);

	void setPassword(std::string);
	void setTopic(std::string);
	void setName(std::string);
	std::string getPassword(void);
	std::string getTopic(void);
	std::string getName(void);

	int addInvite(std::string);
	int isInvite(std::string);
	int delInvite(std::string);

	int addOperator(Client);
	int isOperator(std::string);
	int delOperator(std::string);

	int addUser(Client);
	int isUser(std::string);
	int delUser(std::string);
};
#endif