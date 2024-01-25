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

	void setPassword(const std::string &password);
	void setUsername(const std::string &username);	
	void setNickname(const std::string &nickname);
	void setRealname(const std::string &realname);		
	const std::string &getPassword(void) const;
	const std::string &getUsername(void) const;	
	const std::string &getNickname(void) const;
	const std::string &getRealname(void) const;	
	

private:
	std::string	password;
	std::string	nickname;
	std::string	username;
	std::string	realname;
	int			fd;
	int			last_connect_time;
};
#endif