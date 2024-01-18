#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Channel.hpp"
#include "Client.hpp"

#define BACKLOG 5 /* 어느 크기가 적당할지 */

class Server
{
public:
	Server();
	Server(const Server &ref);
	~Server();

	Server &operator=(const Server &ref);

	bool pasreAndSetArguements(const char * const * argv);

private:
	in_port_t				port;
	std::string				password;
	std::vector<Channel>	channels;
	std::vector<Client>		clients;
	int						max_clients;

	bool init(int &serverSocket) const;
};
#endif