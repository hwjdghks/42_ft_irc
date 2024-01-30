#ifndef DEF_HPP
# define DEF_HPP

# include <vector>
# include <string>

# define BACKLOG 5 /* 어느 크기가 적당할지 */
# define MAX_CLIENT 10
# define REGISTER_TIMEOUT_LIMIT 20
# define CONNECT_TIMEOUT_LIMIT 120
# define PINGPONG_TIMEOUT_LIMIT 5
# define REGISTER_TIMEOUT_LIMIT 20
# define CONNECT_TIMEOUT_LIMIT 120
# define PINGPONG_TIMEOUT_LIMIT 5

# define MAX_CHANNEL 10

# define SUCCESS 1
# define FAIL 0

# define TIMEOUT "time out"
# define SOCKETERR "socket err"

# define HOST "localhost"
# define SERVERNAME	"ft_irc"
# define SERVERURL	"ft_irt.42.fr"

enum {
	PASS, NICK, USER
};

enum {
	TITLE, OP, KEY, LIMIT, INVITE
};

typedef struct s_send_event
{
	bool				recv_work; // 만약 꺼져있다면 아무것도 하지 마
	bool				recv_time; // timestamp 갱신을 해야함
	bool				recv_close; // 이 소켓은 이번 실행 마지막에 종료해야함
	bool				to_send; // send할 것이 있음
	std::vector<int>	fds; // to_send가 참이면 요 벡터의 fd들을 send event에 등록해야행 
}			t_send_event;

struct IRCMessage 
{
	std::string					prefix;
	std::string					command;
	std::vector<std::string>	parameters;
};

#endif
