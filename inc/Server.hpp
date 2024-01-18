#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"

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
};
#endif