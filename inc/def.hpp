#ifndef DEF_HPP
# define DEF_HPP

# include <list>
# include <vector>
# include <string>

#define BACKLOG 5
#define MAX_CLIENT 10
#define MAX_CHANNEL 10
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
#define MSG_FAIL_JOIN_SERVER "ircserv: Server is full\r\n"
#define MSG_FAIL_REGISTER "ircserv: Register timeout\r\n"
#define MSG_FAIL_TIMEOUT "ircserv: Connection lost\r\n"
#define MSG_FAIL_SYSTEM "ircserv: System call error\r\n"

# define SUCCESS 1
# define FAIL 0

# define QUITMSG "Client exited"
# define TIMEOUT "time out"
# define SOCKETERR "socket err"

# define BOTNAME "Manner_bot" // nick 검사할 때 요것도 같이 검사
# define CHATMANNER "Please do not use bad words!"
# define BOTPRIFIX ":" BOTNAME "!ircserv@ircserv.42.fr"

# define HOST "localhost"
# define SERVERNAME	"ircserv"
# define SERVERURL	":" SERVERNAME ".42.fr"

enum {
	PASS, NICK, USER
};

enum {
	TITLE, KEY, LIMIT, INVITE
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
