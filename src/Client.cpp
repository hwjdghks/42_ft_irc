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
	this->addr = ref.addr;
	this->addr_len = ref.addr_len;
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

void Client::setAddr(const sockaddr_in &addr)
{
	this->addr = addr;
}

sockaddr_in &Client::getAddr(void)
{
	return this->addr;
}

void Client::setAddrLen(const socklen_t &len)
{
	this->addr_len = len;
}

socklen_t &Client::getAddrLen(void)
{
	return this->addr_len;
}