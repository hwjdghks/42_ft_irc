#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>

class Client
{
public:
	Client();
	Client(const Client &ref);
	~Client();

	Client &operator=(const Client &ref);
	void setFd(const int &fd);
	const int &getFd(void) const;
	void setAddr(const sockaddr_in &addr);
	sockaddr_in &getAddr(void);
	void setAddrLen(const socklen_t &len);
	socklen_t &getAddrLen(void);

private:
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	realname;
	int			fd;
	sockaddr_in	addr;
	socklen_t	addr_len;
	int			last_connect_time;
};
#endif