#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "def.hpp"

class Channel;

class Client
{
private:
	int						fd;
	bool					bot;
	bool					life;
	bool					regi[3];
	std::string				read_buffer;
	std::string				write_buffer;
	std::string				password;
	std::string				nickname;
	std::string				username;
	std::string				hostname;
	std::string				realname;
	std::vector<Channel *>	channels;
public:
	Client();
	Client(const Client &ref);
	~Client();

	Client &operator=(const Client &ref);

	std::vector<Channel *> &getChannels(void);

	void setBot(const int &bot);
	const bool &getBot(void) const;

	void setFd(const int &fd);
	const int &getFd(void) const;

	void setPassword(const std::string &password);
	void setUsername(const std::string &username);
	void setNickname(const std::string &nickname);
	void setHostname(const std::string &hostname);
	void setRealname(const std::string &realname);
	const std::string &getPassword(void) const;
	const std::string &getUsername(void) const;
	const std::string &getNickname(void) const;
	const std::string &getHostname(void) const;
	const std::string &getRealname(void) const;

	std::string makeClientPrefix(void) const;
	bool isMaxJoin(void);
	
	bool isBot(void);
	bool isAlive(void);
	bool isRegistered(void);
	
	int addRead_buffer(std::string);
	std::string getLineOfRead_buffer(void);
	int delRead_buffer();

	int addWrite_buffer(std::string);
	std::string getWrite_buffer(void);
	int delWrite_buffer();
	void rollbackBuf(std::string buf, ssize_t len);

private:
	int _sendWelcomeMessage();
};

#endif
