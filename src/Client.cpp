#include "Client.hpp"
#include <iostream>
Client::Client(void) {}

Client::Client(const Client &ref)
{
	*this = ref;
}

Client::~Client(void) {}

Client &Client::operator=(const Client &ref)
{
	if (this == &ref)
		return *this;
	/* Edit */
	this->fd = ref.fd;
	return *this;
}

void Client::setFd(const int &fd)
{
	this->fd = fd;
}

const int &Client::getFd(void) const
{
	return this->fd;
}

void Client::setpassword(const std::string &password)
{
	this->password = password;
}

void Client::setusername(const std::string &username)
{
	this->username = username;
}

void Client::setnickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void Client::setrealname(const std::string &realname)
{
	this->realname = realname;
}


const std::string &Client::getpassword(void) const
{
	return this->password;
}

const std::string &Client::getnickname(void) const
{
	return this->nickname;
}

const std::string &Client::getrealname(void) const
{
	return this->realname;
}

const std::string &Client::getusername(void) const
{
	return this->username;
}