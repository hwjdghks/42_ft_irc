#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "def.hpp"

class Channel;

class Client
{
private:
	int						fd;
	bool					life;
	bool					regi[3];
	std::string				read_buffer;
	std::string				write_buffer;
	std::string				password;
	std::string				nickname;
	std::string				username;
	std::string				realname;
	std::vector<Channel *>	channels;
public:
	Client();
	Client(const Client &ref);
	~Client();

	Client &operator=(const Client &ref);

	void setFd(const int &fd);
	const int &getFd(void) const;

	void setPassword(const std::string &password);
	void setUsername(const std::string &username);
	void setNickname(const std::string &nickname);
	void setRealname(const std::string &realname);
	const std::string &getPassword(void) const;
	const std::string &getUsername(void) const;
	const std::string &getNickname(void) const;
	const std::string &getRealname(void) const;
	
	bool isAlive();
	bool isRegistered(void);

	int addWrite_buffer(std::string);
	std::string getWrite_buffer(void);
	int delWrite_buffer(std::string);
};

#endif
