#include "Client.hpp"
#include <iostream>

Client::Client(void) : fd(0), bot(false), life(true)
{
	regi[PASS] = false;
	regi[NICK] = false;
	regi[USER] = false;
}

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
	this->life = ref.life;
	this->regi[PASS] = ref.regi[PASS];
	this->regi[NICK] = ref.regi[NICK];
	this->regi[USER] = ref.regi[USER];
	this->read_buffer = ref.read_buffer;
	this->write_buffer = ref.write_buffer;
	this->password = ref.password;
	this->nickname = ref.nickname;
	this->username = ref.username;
	this->hostname = ref.hostname;
	this->realname = ref.realname;
	this->channels = ref.channels;
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

void Client::setHostname(const std::string &hostname)
{
	this->hostname = hostname;
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

const std::string &Client::getHostname(void) const
{
	return this->hostname;
}

const std::string &Client::getUsername(void) const
{
	return this->username;
}

bool Client::isAlive(void)
{
	return (life);
}

bool Client::isRegistered(void)
{
	return (regi[PASS] && regi[NICK] && regi[USER]);
}


