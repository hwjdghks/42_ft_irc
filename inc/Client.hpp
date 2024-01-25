#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>

enum {
	PASS, NICK, USER
};

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
	int						fd;
	bool					regi[3];
	auto					read_buffer;
	auto					write_buffer;
	std::vector<Channel *>	chans;
	std::string				password;
	std::string				nickname;
	std::string				username;
	std::string				realname;
};
#endif