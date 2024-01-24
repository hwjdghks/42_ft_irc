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

bool Server::addToWaiting(const int &kq, const int &server_socket)
{
	std::cout << "try accept... ";

	const int client_socket = accept(server_socket, NULL, NULL);

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
	if (!this->addReadEvent(kq, client_socket))
	{
		/* kevent Error */
		return false;
	}
	if (!this->addTimerEvent(kq, client_socket, 20))
	{
		/* kevent Error */
		return false;
	}
	Client new_client;
	
	new_client.setFd(client_socket);
	this->waiting_clients.push_back(new_client);
	return true;
}

bool Server::addReadEvent(const int &kq, const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
	{
		/* kevent error */
		return false;
	}
	return true;
}

bool Server::addTimerEvent(const int &kq, const int &fd, int second) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, second * 1000000, NULL);
	if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
	{
		/* kevent error */
		return false;
	}
	return true;
}

bool Server::delTimerEvent(const int &kq, const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
	if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1)
	{
		/* kevent error */
		return false;
	}
	return true;
}

std::vector<Client>::iterator Server::getCurrentClient(int fd, int *loc)
{
	std::vector<Client>::iterator it;
	int pos = -1;

	it = this->searchClient(this->waiting_clients.begin(), this->waiting_clients.end(), fd);
	if (it != waiting_clients.end())
		pos = 1;
	else
	{
		it = this->searchClient(this->clients.begin(), this->clients.end(), fd);
		if (it != clients.end())
			pos = 2;
	}
	if (loc)
		*loc = pos;
	return it;
}

bool Server::delClient(int fd)
{
	int loc;
	std::vector<Client>::iterator it = this->getCurrentClient(fd, &loc);
	std::cout << "find!\n";

	switch (loc)
	{
	case 1:
		/* code */
		this->waiting_clients.erase(it);
		break;
	case 2:
		/* code */
		this->clients.erase(it);
		break;
	default:
		/* Error */
		return false;
	}
	return true;
}

void Server::moveToClients(int kq, int fd)
{
	this->delTimerEvent(kq, fd);
	std::vector<Client>::iterator it = this->searchClient(this->waiting_clients.begin(), this->waiting_clients.end(), fd);
	this->clients.push_back(*it);
	this->waiting_clients.erase(it);
	this->addTimerEvent(kq, fd, 120);
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

	if (!this->addReadEvent(kq, server_socket))
	{
		/* addEvent error */
		std::cout << "fail.\n";
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
					continue ; /* Error handling end */
				}
				if (current_event->ident == server_socket)
				{
					this->addToWaiting(kq, server_socket);
				}
				else
				{
					if (current_event->filter == EVFILT_READ)
					{
						if (current_event->flags & EV_EOF) /* socket close */
						{
							/* Equal to recv() return value is 0 */
							/* need reply? */
							close(current_event->ident);
							this->delClient(current_event->ident);
							continue ; /* Error handling end */
						}
						char buf[4096];
						int size = recv(current_event->ident, &buf, sizeof(buf), 0);
						if (size == -1)
						{
							/* recive error */
							std::cout << "recv fail.\n";
							break ;
						}
						else
						{
							buf[size] = '\0';
							// 명령어 파싱
							// 명령어 분기
							int loc;
							Client &current_client = *this->getCurrentClient(current_event->ident, &loc);
							switch (loc)
							{
							case 1: /* waiting list */
								/* code */
								break;
							case 2: /* join client list */
								/* code */
								break;
							default: 
								/* Error */
								break;
							}
						}
					}
					else
					{
						std::cout << "something happened..\n";
					}
				}
			} /* event loop end */
			break ;
		} /* switch case end */
	} /* infinite loop end */
}