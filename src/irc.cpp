#include "irc.h"
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

IRCConnection::IRCConnection() {

}

IRCConnection::IRCConnection(std::string host, int port) {
	this->Start(host, port, "ircbot");
}

IRCConnection::IRCConnection(std::string host, int port, std::string name) {
	this->Start(host, port, name);
}

IRCConnection::IRCConnection(std::string host, int port, std::string nickname,
				std::string username, std::string fullname) {
	this->Start(host, port, nickname, username, fullname);
}

IRCConnection::~IRCConnection() {
	this->tcp.Disconnect();
}

void IRCConnection::Start(std::string host, int port, std::string nickname) {
#ifdef DEBUG
	printf("[DEBUG] Starting IRCConnection:\n\
[DEBUG] Host:Port    %s:%d\n\
[DEBUG] Username     %s\n\
[DEBUG] Nickname     %s\n\
[DEBUG] Real name    %s\n",
	host.c_str(), port, nickname.c_str(), nickname.c_str(), nickname.c_str());
#endif
	this->tcp.Connect(host, port);
	this->thr = std::thread(IRCConnection::mainLoopThread, this);
	this->CmdUser(nickname, nickname);
	this->CmdNick(nickname);
}

void IRCConnection::Start(std::string host, int port, std::string nickname,
			std::string username, std::string fullname) {
	this->tcp.Connect(host, port);
	this->CmdUser(username, fullname);
	this->CmdNick(nickname);
}

void IRCConnection::Stop() {
	this->done = true;
	this->thr.join();
}

void IRCConnection::JoinThread() {
	this->thr.join();
}

void IRCConnection::mainLoopThread(IRCConnection *irc) {
	while (true) {
		if (irc->txqueue.size() > 0) {
			// Send a line from the txqueue
			irc->tcp.WriteLine(irc->txqueue.front());
			irc->txqueue.pop();
		}
		
		std::string line = irc->tcp.ReadLine();
		boost::cmatch m;
		if (regex_match(line.c_str(), m, boost::regex("^:[^ ]+ ([0-9]+) [^ ]+ (.*)"))) {
			irc->eventHandler->onNumericReceived(boost::lexical_cast<int>(m[1].str()), m[2].str());
		} else if (regex_match(line.c_str(), m, boost::regex("^PING :(.+)"))) {
			if (irc->eventHandler->onPingReceived(m[1].str()))
				irc->txqueue.push("PONG :" + m[1].str());
		}
	}
}


void IRCConnection::CmdJoin(std::string channel) {
	this->txqueue.push("JOIN " + channel);
}

void IRCConnection::CmdNick(std::string nick) {
	this->txqueue.push("NICK " + nick);
}

void IRCConnection::CmdPart(std::string channel) {
	this->txqueue.push("PART " + channel);
}

void IRCConnection::CmdPart(std::string channel, std::string reason) {
	this->txqueue.push("PART " + channel + " :" + reason);
}

void IRCConnection::CmdQuit() {
	this->txqueue.push("QUIT");
}

void IRCConnection::CmdQuit(std::string reason) {
	this->txqueue.push("QUIT :" + reason);
}

void IRCConnection::CmdUser(std::string username, std::string realname) {
	this->txqueue.push("USER " + username + " 8 * :" + realname);
}
