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

void Client::setPassword(const std::string &password)
{
	this->password = password;
}

void Client::setUsername(const std::string &username)
{
	this->username = username;
}

void Client::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void Client::setRealname(const std::string &realname)
{
	this->realname = realname;
}


const std::string &Client::getPassword(void) const
{
	return this->password;
}

const std::string &Client::getNickname(void) const
{
	return this->nickname;
}

const std::string &Client::getRealname(void) const
{
	return this->realname;
}

const std::string &Client::getUsername(void) const
{
	return this->username;

void Client::setTime(void)
{
	this->last_connect_time = std::time(NULL);
}

time_t Client::getTime(void) const
{
	return this->last_connect_time;
}