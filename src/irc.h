#ifndef IRC_H_GUARD
#define IRC_H_GUARD

#include "socket.h"
#include <thread>
#include <queue>

struct IRCUser {
	std::string Nickname;
	std::string Username;
	std::string Host;
};

class EventHandler {
public:
	virtual void onMessageReceived(IRCUser from, std::string channel, std::string message) { };
	virtual void onNumericReceived(int numeric, std::string message) { };
	virtual bool onPingReceived(std::string code) { return true; };
};

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
	void JoinThread();

	void CmdJoin(std::string channel);
	void CmdNick(std::string nick);
	void CmdPart(std::string channel);
	void CmdPart(std::string channel, std::string reason);
	void CmdQuit();
	void CmdQuit(std::string reason);
	void CmdUser(std::string username, std::string realname);
	
	EventHandler *eventHandler = NULL;

private:
	TCPClient tcp;
	std::thread thr;
	bool done;
	std::queue<std::string> txqueue;
	
	static void mainLoopThread(IRCConnection *irc);
};

#endif
