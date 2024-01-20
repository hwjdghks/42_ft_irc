#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client
{
public:
	Client();
	Client(const Client &ref);
	~Client();

	Client &operator=(const Client &ref);
	void setFd(const int &fd);
	const int &getFd(void) const;

private:
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	realname;
	int			fd;
	int			last_connect_time;
};
#endif