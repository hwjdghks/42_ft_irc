#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

#include "Channel.hpp"
#include "Client.hpp"

#define BACKLOG 5 /* 어느 크기가 적당할지 */
#define MAX_CLIENT 10

struct IRCMessage 
{
	std::string prefix;
	std::string command;
	std::vector <std::string> parameters;
};

class Server
{
public:
	Server();
	Server(const Server &ref);
	~Server();

	Server &operator=(const Server &ref);

	bool pasreAndSetArguements(const char * const * argv);
	void run(void);

private:
	in_port_t				port;
	std::string				password;
	std::vector<Channel>	channels;
	std::vector<Client>		clients;
	std::vector<Client>		waiting_clients;
	int						max_clients;

	bool init(int &serverSocket) const;
	bool addEvent(const int &kq, struct kevent &change_event, const int &fd) const;
	bool addClient(const int &kq, const int &server_socket, struct kevent &change_event);
	bool delClient(int fd);
	const std::vector<Client>::const_iterator searchClient(int fd) const;
	std::string str_toupper(std::string s);
	IRCMessage parseMessage(const char message[]);
};
#endif