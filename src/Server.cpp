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

bool Server::init(void)
{
	signal(SIGPIPE, SIG_IGN);
	control.setPassword(this->password);
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		return false;
	int flag = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
		return closeServerSocket();
	struct sockaddr_in server_addr;

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(this->port);
	if (bind(server_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr)) == -1)
		return closeServerSocket();
	if (listen(server_fd, BACKLOG) == -1)
		return closeServerSocket();
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
		return closeServerSocket();
	this->kq = kqueue();
	if (kq == -1)
		return closeServerSocket();
	if (!addReadEvent(server_fd))
	{
		closeKqueue();
		return closeServerSocket();
	}
	return true;
}

void Server::run(void)
{
	if (!init())
		return ;
	std::vector<struct kevent> event_list;

	event_list.reserve(MAX_CLIENT + 1);
	while (true)
	{
		timespec ts;
		ts.tv_sec = 2;
		ts.tv_nsec = 0;
		int new_events_count = kevent(kq, NULL, 0, &event_list[0], MAX_CLIENT + 1, &ts);
		if (new_events_count == -1)
		{
			/* kevent error */
			return this->stop();
		}
		else if (new_events_count == 0)
		{}
		else
		{
			for (int idx = 0; idx < new_events_count; idx++)
			{
				struct kevent *current_event = &event_list[idx];
				if (current_event->flags & EV_ERROR)
				{
					if (current_event->ident == static_cast<uintptr_t>(this->server_fd))
					{
						/* server socket error */
						return this->stop();
					}
					else
					{
						/* client socket error */
						if (!closeClient(current_event->ident, MSG_FAIL_SYSTEM))
							return this->stop();
					}
					continue ;
				}
				if (current_event->ident == static_cast<uintptr_t>(this->server_fd))
				{
					if (!setNewClientSocket())
						return this->stop();
				}
				else if (current_event->filter == EVFILT_READ)
				{
					if (current_event->flags & EV_EOF) /* peer connect close */
					{
						if (!closeClient(current_event->ident, MSG_FAIL_SYSTEM))
							return this->stop();
					}
					else if (!recvMsg(current_event->ident))
						return this->stop();
				}
				else if (current_event->filter == EVFILT_WRITE)
				{
					if (current_event->flags & EV_EOF) /* peer connect close */
					{
						/* socket close */
						if (!closeClient(current_event->ident, MSG_FAIL_SYSTEM))
							return this->stop(); /* kevent error */
					}
					else if (!sendMsg(current_event->ident, current_event->udata))
						return this->stop();
				}
				else if (current_event->filter == EVFILT_TIMER)
				{
					/* register, connect 등 이벤트 처리 */
					if (!handleTimerEvent(current_event->ident, current_event->udata))
						return this->stop();
				}
			} /* each event handling end */
		} /* kevent case end */
	} /* infinite loop end */
}
