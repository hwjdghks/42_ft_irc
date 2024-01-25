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

void Client::setTime(void)
{
	this->last_connect_time = std::time(NULL);
}

time_t Client::getTime(void) const
{
	return this->last_connect_time;
}