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

std::vector<Channel *> &Client::getChannels(void)
{
	return this->channels;
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

bool Client::isBot(void)
{
	return (bot);
}

bool Client::isRegistered(void)
{
	return (regi[PASS] && regi[NICK] && regi[USER]);
}

std::string Client::makeClientPrefix() const
{
    return this->getNickname() + "!" + \
           this->getUsername() + "@" + \
           this->getHostname();

int Client::addRead_buffer(std::string recv_buffer)
{
	this->read_buffer += recv_buffer;
	return (SUCCESS);
}

std::string Client::getLineOfRead_buffer(void)
{
	std::string res;
	size_t loc;

	res.clear();
	loc = read_buffer.find('\n');
	if (loc != std::string::npos)
	{
		res = read_buffer.substr(0, loc);
		read_buffer = read_buffer.substr(loc + 1, read_buffer.size() - loc);
	}
	return (res);
}

int Client::delRead_buffer()
{
	read_buffer.clear();
	return (SUCCESS);
}

int Client::addWrite_buffer(std::string send_buffer)
{
	this->write_buffer += send_buffer;
	return (SUCCESS);
}

std::string Client::getWrite_buffer(void)
{
	return (write_buffer);
}

int Client::delWrite_buffer()
{
	write_buffer.clear();
	return (SUCCESS);
}

void Client::rollbackBuf(std::string buf, ssize_t len)
{
	write_buffer = write_buffer.substr(len + 1, read_buffer.size() - len);
}

bool Client::isMaxJoin(void)
{
	return (MAX_CHANNEL == this->channels.size());
}