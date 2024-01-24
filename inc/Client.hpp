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

	void setpassword(const std::string &password);
	void setusername(const std::string &username);	
	void setnickname(const std::string &nickname);
	void setrealname(const std::string &realname);		
	const std::string &getpassword(void) const;
	const std::string &getusername(void) const;	
	const std::string &getnickname(void) const;
	const std::string &getrealname(void) const;	
	

private:
	std::string	password;
	std::string	nickname;
	std::string	username;
	std::string	realname;
	int			fd;
	int			last_connect_time;
};
#endif