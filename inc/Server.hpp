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
#define REGISTER_TIMEOUT_LIMIT 20
#define CONNECT_TIMEOUT_LIMIT 120
#define PINGPONG_TIMEOUT_LIMIT 5
#define REGISTER_TIMEOUT_LIMIT 20
#define CONNECT_TIMEOUT_LIMIT 120
#define PINGPONG_TIMEOUT_LIMIT 5

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
	int						server_fd;
	int						kq;
	in_port_t				port;
	std::string				password;
	std::vector<Channel>	channels;
	std::vector<Client>		clients;
	int						max_clients;

	bool init(int &serverSocket) const;
	bool addReadEvent(const int &kq, const int &fd) const;
	bool addTimerEvent(const int &kq, const int &fd, int second) const;
	bool delTimerEvent(const int &kq, const int &fd) const;
	bool addToWaiting(const int &kq, const int &server_socket);
	bool addClient(const int &kq, const int &server_socket, struct kevent &change_event);
	bool delClient(int fd);
	IRCMessage parseMessage(const char message[]);

	void moveToClients(int kq, int fd);
    std::vector<Client>::iterator getCurrentClient(int fd, int *loc);
	template <typename Iter>
	Iter searchClient(Iter first, Iter last, int fd) const
	{
		for(; first != last; ++first)
		 	if (first->getFd() == fd)
				break ;
		return first;
	}
	void stop(int kq, int server_socket);

private:
	std::string str_toupper(std::string s);
	bool isValidChar(const char c);
	bool isValidNick(const std::string &str);
	bool isDupNick(Client* cur_client, const std::string &nick);
	void handleMessage(const IRCMessage &message, const int &fd);

	void handleUSERcmd(Client* client, const std::vector<std::string>& parameters);
	void handlePASScmd(Client* client, const std::vector<std::string>& parameters);
	void handleNICKcmd(Client* client, const std::vector<std::string>& parameters);

	void sendMSG(Client* receiver, struct IRCMessage send_message);
	void sendMSG(Channel* recive_channel, struct IRCMessage send_message);

};
#endif