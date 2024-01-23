#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <ctime>

class Client
{
public:
	Client();
	Client(const Client &ref);
	~Client();

	Client &operator=(const Client &ref);
	void setFd(const int &fd);
	const int &getFd(void) const;
	void setTime(void);
	time_t getTime(void) const;

private:
	std::string	password;
	std::string	nickname;
	std::string	username;
	std::string	realname;
	int			fd;
	time_t		last_connect_time;
};
#endif