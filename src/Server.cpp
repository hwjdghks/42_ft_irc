#include "Server.hpp"
#include <iostream> /* test header */

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

std::string Server::str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

IRCMessage Server::parseMessage(const char message[])
{
	IRCMessage ircMessage;
	
	std::istringstream iss(message);
	std::string token;

// Extracting prefix
if (message[0] == ':') 
	{
	iss >> token;
	ircMessage.prefix = token.substr(1);
	}

// Extracting command
iss >> token;
ircMessage.command = str_toupper(token);

// Extracting parameters
while (iss >> token) 
{
	ircMessage.parameters.push_back(token);
}

return ircMessage;
}


bool Server::addClient(const int &kq, const int &server_socket, struct kevent &change_event)
{
	std::cout << "try accept... ";

	sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	const int client_socket = accept(server_socket, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);

	std::cout << "fd set: " << client_socket << ' ';
	if (client_socket == -1)
	{
		std::cout << "fail.\n";
		/* Error */
		return false;
	}
	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fail.\n";
		/* Error */
		return false;
	}
	if (!this->addEvent(kq, change_event, client_socket))
	{
		/* kevent Error */
		return false;
	}
	Client new_client;
	
	new_client.setFd(client_socket);
	this->clients.push_back(new_client);
	return true;
}

bool Server::addEvent(const int &kq, struct kevent &change_event, const int &fd) const
{
	std::memset(&change_event, 0, sizeof(change_event));
	EV_SET(&change_event, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	std::cout << "set client socket to kevent... " << std::flush;
	if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
	{
		/* kevent error */
		return false;
	}
	return true;
}

const std::vector<Client>::const_iterator Server::searchClient(int fd) const
{
	std::vector<Client>::const_iterator it;

	for(it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->getFd() == fd)
			break ;
	return it;
}

bool Server::delClient(int fd)
{
	std::vector<Client>::const_iterator it = this->searchClient(fd);

	if (it == this->clients.end())
	{
		/* Error */
		return false;
	}
	this->clients.erase(it);
	return true;
}

bool Server::init(int &server_socket) const
{
	std::cout << "Create Server socket... ";
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		/* socket error */
		std::cout << "fail.\n";
		return false;
	}
	std::cout << "Success!!\n";
	int flag = 1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
		/* setsockopt rror */
		std::cout << "fail.\n";
		return false;
	}
	struct sockaddr_in server_addr;

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(this->port);
	std::cout << "try bind... ";
	if (bind(server_socket, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
	{
		/* bind error */
		std::cout << "fail.\n";
		if (close(server_socket) == -1)
		{
			/* close error */
			return false;
		}
		return false;
	}
	std::cout << "Success!!\n";
	std::cout << "try listen... ";
	if (listen(server_socket, BACKLOG) == -1)
	{
		/* listen error */
		std::cout << "fail.\n";
		if (close(server_socket) == -1)
		{
			/* close error */
			return false;
		}
		return false;
	}
	std::cout << "Success!!\n";
	std::cout << "set non_block ... ";
	if (fcntl(server_socket, F_SETFL, O_NONBLOCK) == -1)
	{
		/* fcntl error */
		std::cout << "fail.\n";
		if (close(server_socket) == -1)
		{
			/* close error */
			return false;
		}
		return false;
	}
	std::cout << "Success!!\n";
	return true;
}

void Server::run(void)
{
	int get_server_socket;

	if (!this->init(get_server_socket))
		return ;
	const int server_socket = get_server_socket;

	std::cout << "create kqueue... ";
	int kq = kqueue();

	if (kq == -1)
	{
		/* kqueue error */
		std::cout << "fail.\n";
		if (close(server_socket) == -1)
		{
			/* close error */
			return ;
		}
		return ;
	}
	std::cout << "Success!!\n";
	/* server socket event apply */
	struct kevent change_event;

	if (!this->addEvent(kq, change_event, server_socket))
	{
		/* addEvent error */
		/* sample code */
		std::cout << "fail.\n";
		close(kq);
		close(server_socket);
		return ;
	}
	std::cout << "Success!!\n";
	std::vector<struct kevent> event_list;

	event_list.reserve(MAX_CLIENT + 1);
	while (true)
	{
		std::cout << "wait event... ";

		timespec ts;
		ts.tv_sec = 2;
		ts.tv_nsec = 0;
		int new_events_count = kevent(kq, NULL, 0, &event_list[0], MAX_CLIENT + 1, &ts);
		switch (new_events_count)
		{
		case -1: /* Error occur */
			std::cout << "fail.\n";
			/* code */
			break ;
		case 0: /* kevent timeout */
			std::cout << "timeout.\n";
			/* code */
			break ;
		default:
			std::cout << "Success!!\n";

			struct kevent *current_event;
			struct IRCMessage message;
			for (int idx = 0; idx < new_events_count; idx++)
			{
				current_event = &event_list[idx];
				if (current_event->flags & EV_ERROR)
				{
					std::cout << "ERROR FLAG SET.\n";
					/* Error */
					if (current_event->ident == server_socket)
					{}
					else
					{}
					break ; /* <= test code */
				}
				else if (current_event->ident == server_socket)
				{
					this->addClient(kq, server_socket, change_event);
				}
				else
				{
					char buf[4096];

					if (current_event->filter == EVFILT_READ)
					{
						int size = recv(current_event->ident, &buf, sizeof(buf), 0);
						if (size == -1)
						{
							/* recive error */
							std::cout << "fail.\n";
							break ;
						}
						else if (size == 0) /* peer has performed an orderly shutdown */
						{
							// EV_SET(&change_event, current_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
							// kevent(kq, &change_event, 1, NULL, 0, NULL);
							close(current_event->ident); /* event 삭제 없이 close만 해도 되는가 */
							this->delClient(current_event->ident);
						}
						else
						{
							buf[size] = '\0';
							std::cout << "from client fd: " << current_event->ident << ": " << buf << std::endl;
							// 명령어 파싱
							message = parseMessage(buf);

							// 명령어 분기
							for (std::size_t idx = 0; idx <clients.size(); idx++)
							{
								Client &current_client = this->clients[idx];
								if (current_client.getFd() == current_event->ident)
									continue ;
								/* send message to other clients */
								std::cout << "send to fd: " << current_client.getFd() << '\n';
								std::string sender;
								sender = "send fd: ";
								sender += std::to_string(current_event->ident);
								sender += ": ";
								sender += buf;
								send(current_client.getFd(), sender.c_str(), strlen(sender.c_str()), 0);
							}
						}
					}
					else
					{
						std::cout << "something happened..\n";
					}
				}
				//clear message(?) -- consider message struct value reset especially vector clear  
			} /* event loop end */
			break ;
		} /* switch case end */
	} /* infinite loop end */
}