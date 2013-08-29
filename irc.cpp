#include "irc.h"

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
	this->tcp.Connect(host, port);
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
