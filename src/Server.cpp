#include "Server.hpp"

Server::Server(void) {}

Server::Server(const Server &ref)
{
	*this = ref;
}

Server::~Server(void) {}

Server &Server::operator=(const Server &ref)
{
	if (this == &ref)
		return *this;
	/* Edit */
	return *this;
}

bool Server::pasreAndSetArguements(const char * const * argv)
{
	long confirm;

	for(const char *iter = argv[1]; iter && *iter; iter++)
		if (!std::isdigit(*iter))
			return false;
	confirm = std::strtol(argv[1], NULL, 10);
	if (confirm < 0 || confirm > 65535)
		return false;
	this->port = static_cast<in_port_t>(confirm);
	for(const char *iter = argv[2]; iter && *iter; iter++)
		if (!std::isprint(*iter))
			return false;
	this->password = std::string(argv[2]);
	return true;
}

bool Server::init(int &server_socket) const
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		/* Error msg */
		return false;
	}
	struct sockaddr_in server_addr;

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(this->port);
	if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
	{
		/* Error msg */
		if (close(server_socket) == -1)
		{
			/* Error msg */
			return false;
		}
		return false;
	}
	if (listen(server_socket, BACKLOG) == -1)
	{
		/* Error msg */
		if (close(server_socket) == -1)
		{
			/* Error msg */
			return false;
		}
		return false;
	}
}
