#include "Server.hpp"

/************************************************************
 *															*
 *						Utility Function					*
 * 															*
 ************************************************************/

/* 프로그램 실행시 <port> <passwd>를 각각 올바른 타입으로 들어왔는지 확인 후 저장 */
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

/*
 * 클라이언트 소켓의 연결 요청을 수락하고 기본 이벤트를 설정
 * 만약 에러가 발생하면 fd를 닫아 연결이 실패한 것으로 행동
*/
bool Server::setNewClientSocket(void)
{
	struct sockaddr_in info;
	socklen_t len = sizeof(info);
	int client_fd = accept(this->server_fd, reinterpret_cast<sockaddr *>(&info), &len);
	if (client_fd == -1)
		return true;
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		/* setting fail. close client */
		close(client_fd);
		return true;
	}
	/* 연결이 max면 클라이언트에 에러를 전송하기 위한 세팅 */
	if (control.getClientListSize() == MAX_CLIENT)
	{
		if (!this->addCustomWriteEvent(client_fd, UDATA_FAIL_JOIN_SERVER))
			return false;
		return true;
	}
	if (!this->setRegisterEvent(client_fd))
	{
		/* event apply fail. close client */
		close(client_fd);
		return false;
	}
	control.createClient(client_fd, std::string(inet_ntoa(info.sin_addr)));
	return true;
}

/*
 * 이벤트가 발생한 fd에 대한 후속 이벤트 설정 및 영향을 받는 fd에 대한 이벤트 설정
 * 명령어 성공시 타이머 이벤트 갱신
 * fd에 해당하는 클라이언트를 닫아야할 시 커스텀 이벤트 세팅 => quit으로 수정해야하나?
 * 영향받는 fd들이 존재 시, 해당 fd 리스트를 순회하며 쓰기 이벤트 켜기
 */
bool Server::setReplyEventToClient(const int &fd, const t_send_event &list) 
{
	if (list.recv_work)
	{
		if (list.recv_time && !addTimerEvent(fd, TIMEOUT_CONNECT, UDATA_CHECK_CONNECT))
			return false;
		if (list.recv_close && !addCustomWriteEvent(fd, UDATA_NEED_QUIT))
			return false;
	}
	if (list.to_send)
		for (std::size_t idx = 0; idx < list.fds.size(); idx++)
			if (!onWriteEvent(list.fds[idx]))
				return false;
	return true;
}

/************************************************************
 *															*
 *				Recv, Send, Timer Function					*
 * 															*
 ************************************************************/

/*
 * 메세지 수신 시퀀스
 * 버퍼의 사이즈만큼 fd에서 읽기 시도
 * 시스템 콜 에러 발생시 해당 소켓 닫음
 * string으로 변환 후 데이터 전송 후 결과 반환
 * 리턴받은 결과를 바탕으로 이벤트 세팅
 * 이벤트 세팅 에러 -> kevent 에러 -> event기반 처리 신뢰도 저하 -> 서버 종료
 */
bool Server::recvMsg(const int &fd)
{
	char buf[BUFSIZE];

	std::memset(buf, 0, BUFSIZE);
	ssize_t size = recv(fd, buf, BUFSIZE - 1, 0);
	if (size == -1)
		return closeClient(fd, MSG_FAIL_SYSTEM);
	buf[size] = '\0';
	t_send_event change_list = control.executeCommand(fd, std::string(buf));

	return this->setReplyEventToClient(fd, change_list);
}

/*
 * 메세지 전송 시퀀스
 * 클라이언트 없이 소켓만 열린 채로 메세지를 보내야할 경우 먼저 검색 (예외 사항)
 * 일반적인 상황의 메세지 전송 시도
 * 시스템 콜 에러 발생시 해당 소켓 닫음
 * 버퍼를 전부 보내지 못했다면 남은 사이즈를 원래의 버퍼로 롤백
 * 버퍼를 전부 보냈다면 write 이벤트를 끈다
 * 메세지를 보낸 클라이언트가 삭제되어야하면 quit 호출
 * 이후 소켓을 닫기 직전 reply를 위한 마지막 write 이벤트 등록
 * 이벤트 세팅 에러 -> kevent 에러 -> event기반 처리 신뢰도 저하 -> 서버 종료
 */
bool Server::sendMsg(const int &fd, void *udata)
{
	if (sendSpecialMsg(fd, udata))
		return true;

	Client *sender = control.searchClient(fd);
	std::string buf = sender->getWrite_buffer(/* 버퍼를 받음. 버퍼 내부는 비워짐 */);
	ssize_t len = send(fd, buf.c_str(), buf.size(), 0);
	if (len == -1)
		return closeClient(fd, MSG_FAIL_SYSTEM);
	if (static_cast<std::size_t>(len) < buf.size())
	{
		sender->rollbackBuf(buf, len);
	}
	else if (!offWriteEvnet(fd))
		return false;
	if ((!sender->isAlive())) /* 클라이언트를 종료시켜야할 경우 */
		return closeClient(fd);
	return true;
}

bool Server::handleTimerEvent(const int &fd, void *udata)
{
	t_send_event event;
	Client *client = NULL;
	intptr_t event_case = reinterpret_cast<intptr_t>(udata);

	switch (event_case)
	{
	case UDATA_CHECK_REIGISTER: /* 등록 시간제한이 지났을 경우 */
		client = control.searchClient(fd);
		if (client && (!client->isAlive() || !client->isRegistered()))
			if (!closeClient(fd, MSG_FAIL_REGISTER))
				return false;
		break;
	case UDATA_CHECK_CONNECT: /* ping을 보내야 함 */
		event = control.ping(fd);
		if (!this->setReplyEventToClient(fd, event))
			return false;
		addTimerEvent(fd, TIMEOUT_CONNECT, UDATA_LOST_CONNECT);
		break;
	case UDATA_LOST_CONNECT: /* ping에 응답이 없을 경우 */
		if (!closeClient(fd, MSG_FAIL_TIMEOUT))
			return false;
		break;
	default:
		break;
	}
	return true;
}

/*
 * write 이벤트를 받은 이후 한번 거침
 * 클라이언트가 없는 상황에서의 마지막 메세지 처리
 * const string을 전송한다.
 * 관리할 인스턴스가 없으므로 소켓을 바로 닫는다.
 */
bool Server::sendSpecialMsg(const int &fd, void *udata)
{
	intptr_t event_case = reinterpret_cast<intptr_t>(udata);

	switch (event_case)
	{
	case UDATA_FAIL_JOIN_SERVER: /* 인원 제한으로 서버 등록 절차가 거부된 경우 */
		send(fd, MSG_FAIL_JOIN_SERVER, sizeof(MSG_FAIL_JOIN_SERVER), 0);
		break;
	/* 더 추가될 경우 기입 */
	default:
		return false;
	}
	close(fd);
	return true;
}

/************************************************************
 *															*
 *					Event manage Function					*
 * 															*
 ************************************************************/

bool Server::setEvent(const struct kevent *change_event) const
{
	if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1)
		return false;
	return true;
}

/*
 * 유저 등록시 필요한 이벤트 세팅
 * 읽기, 쓰기(비활성), 등록 제한시간 (20초)
 * 이벤트 세팅 에러 -> kevent 에러 -> event기반 처리 신뢰도 저하 -> 서버 종료
 */
bool Server::setRegisterEvent(const int &fd) const
{
	if (!addReadEvent(fd))
		return false;
	if (!addWriteEvent(fd))
		return false;
	if (!offWriteEvnet(fd))
		return false;
	if (!addTimerEvent(fd, TIMEOUT_REGISTER, UDATA_CHECK_REIGISTER))
		return false;
	return true;
}

bool Server::addReadEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::addWriteEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::addCustomWriteEvent(const int &fd, int purpose) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_WRITE, EV_ADD, 0, 0, reinterpret_cast<void *>(purpose));
	return this->setEvent(&change_event);
}

bool Server::addTimerEvent(const int &fd, int second, int purpose) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_TIMER, EV_ADD | EV_ONESHOT, 0, second * 1000, reinterpret_cast<void *>(purpose));
	return this->setEvent(&change_event);
}

bool Server::onReadEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_READ, EV_ENABLE, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::onWriteEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_WRITE, EV_ENABLE, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::offReadEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::offWriteEvnet(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_WRITE, EV_DISABLE, 0, 0, NULL);
	return this->setEvent(&change_event);
}

bool Server::delTimerEvent(const int &fd) const
{
	struct kevent change_event;

	EV_SET(&change_event, fd, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
	return this->setEvent(&change_event);
}

/************************************************************
 *															*
 *					Error handle Function					*
 * 															*
 ************************************************************/

bool Server::closeServerSocket(void) const
{
	close(this->server_fd);
	return false;
}

bool Server::closeKqueue(void) const
{
	close(this->kq);
	return false;
}

bool Server::closeClient(int fd)
{
	if (!setReplyEventToClient(fd, control.deleteClient(fd)))
		return false;
	delTimerEvent(fd);
	close(fd);
	return true;
}

bool Server::closeClient(int fd, const char *msg)
{
	if (!setReplyEventToClient(fd, control.quit(fd, msg)))
		return false;
	delTimerEvent(fd);
	close(fd);
	return true;
}

void Server::stop(void)
{
	/* delete all client and close client socket */
	std::vector<int> fds = control.getAllClientFd();
	for(std::size_t idx = 0; idx < fds.size(); idx++)
		close(fds[idx]);
	close(kq);
	close(server_fd);
}
