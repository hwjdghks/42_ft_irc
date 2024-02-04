#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "def.hpp"
#include "Client.hpp"

class Channel
{
private:
	bool					option[4];
	int						limit;
	Client					bot;
	std::string				password;
	std::string				topic;
	std::string				name;
	std::vector<Client *>	operators;
	std::vector<Client *>	users;
	std::vector<Client *>	invited;
public:
	Channel();
	Channel(const Channel &ref);
	~Channel();

	Channel &operator=(const Channel &ref);

	void setOptionTitle(bool);
	void setOptionkey(bool);
	void setOptionLimit(bool);
	void setOptionInvite(bool);

	bool getOptionTitle(void);
	bool getOptionkey(void);
	bool getOptionLimit(void);
	bool getOptionInvite(void);

	std::vector<Client *> &getOperators(void);
	std::vector<Client *> &getUsers(void);

	void setPassword(std::string);
	void setTopic(std::string);
	void setName(std::string);
	std::string getPassword(void);
	std::string getTopic(void);
	std::string getName(void);

	bool addInvite(Client &client);
	bool isInvite(const std::string &nickname);
	void delInvite(const std::string &nickname);

	bool addOperator(Client &client);
	bool isOperator(const std::string &nickname) const;
	void delOperator(const std::string &nickname);

	bool addUser(Client &client);
	bool isUser(const std::string &nickname) const;
	void delUser(const std::string &nickname);
};
#endif