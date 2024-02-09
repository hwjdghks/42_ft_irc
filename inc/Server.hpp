#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>
#include <fcntl.h>

#include "Irc.hpp"

class Server
{
public:
	Server();
	~Server();

	bool pasreAndSetArguements(const char * const * argv);
	void run(void);

private:
	/* NO SUPPORT COPY */
	Server(const Server &ref);
	Server &operator=(const Server &ref);

private:
	int			server_fd;
	int			kq;
	in_port_t	port;
	std::string	password;
	Irc			control;

private: /* utility */
	bool init(void);
	bool setNewClientSocket(void);
	bool setReplyEventToClient(const int &fd, const t_send_event &list);

private: /* recv, send, timer */
	bool recvMsg(const int &fd);
	bool sendMsg(const int &fd, void *udata);
	bool handleTimerEvent(const int &fd, void *udata);
	bool sendSpecialMsg(const int &fd, void *udata);

private: /* manage kevent */
	bool setEvent(const struct kevent *change_event) const;
	bool setRegisterEvent(const int &fd) const;

	bool addReadEvent(const int &fd) const;
	bool addWriteEvent(const int &fd) const;
	bool addCustomWriteEvent(const int &fd, int purpose) const;
	bool addTimerEvent(const int &fd, int time, int purpose) const;

	bool onReadEvent(const int &fd) const;
	bool onWriteEvent(const int &fd) const;

	bool offReadEvent(const int &fd) const;
	bool offWriteEvnet(const int &fd) const;

	bool delTimerEvent(const int &fd) const;

private: /* error handling */
	bool closeServerSocket(void) const;
	bool closeKqueue(void) const;
	bool closeClient(int fd, const char *msg);
	void stop(void);

private: /* debug */
	void printData(struct kevent *e) const;
};
#endif