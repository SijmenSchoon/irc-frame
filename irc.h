#ifndef IRC_H_GUARD
#define IRC_H_GUARD

#include "socket.h"
#include <threads>

class IRCConnection {
public:
	IRCConnection();
	IRCConnection(std::string host, int port);
	IRCConnection(std::string host, int port, std::string nickname);
	IRCConnection(std::string host, int port, std::string nickname, std::string username, std::string fullname);
	~IRCConnection();

	void Start(std::string host, int port, std::string nickname);
	void Start(std::string host, int port, std::string nickname, std::string username, std::string fullname);
	void Stop();

	void CmdJoin(std::string channel);
	void CmdNick(std::string nick);
	void CmdPart(std::string channel);
	void CmdPart(std::string channel, std::string reason);
	void CmdUser(std::string username, std::string realname);

private:
	TCPClient tcp;
	std::thread thr;
};

#endif
