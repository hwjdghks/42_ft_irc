#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <unistd.h>
#include <fcntl.h>

#include "Irc.hpp"

#define BACKLOG 5 /* 어느 크기가 적당할지 */
#define MAX_CLIENT 10
#define BUFSIZE 4096

/* 각 클라이언트의 단계별 timeout 기본 세팅 */
#define TIMEOUT_REGISTER 20
#define TIMEOUT_CONNECT 120

/* timer event 경우의 수 체크 */
#define UDATA_CHECK_REIGISTER 1
#define UDATA_CHECK_CONNECT 2
#define UDATA_LOST_CONNECT 3
#define UDATA_NEED_QUIT 4
#define UDATA_FAIL_JOIN_SERVER 5
#define UDATA_FAIL_REGISTER 6

/* 클라이언트 종료시 종료 이유를 보낼 다잉 메세지 */
static const char *MSG_FAIL_JOIN_SERVER = "Server is full";
static const char *MSG_FAIL_REGISTER = "Register timeout";
static const char *MSG_FAIL_TIMEOUT = "Connection lost";
static const char *MSG_FAIL_SYSTEM = "System call error";

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
	void stop(void);

private: /* debug */
	void printData(struct kevent *e) const;
};
#endif