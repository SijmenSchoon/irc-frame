#include "irc.h"

IRCConnection::IRCConnection() {

}

IRCConnection::IRCConnection(std::string host, int port) {
	this->Connect(host, port);
}

IRCConnection::IRCConnection(std::string host, int port, std::string name) {
	this->Connect(host, port, name);
}

IRCConnection::IRCConnection(std::string host, int port, std::string nickname,
				std::string username, std::string fullname) {
	this->Connect(host, port, nickname, username, fullname);
}

IRCConnection::~IRCConnection() {
	this->Disconnect();
}

void IRCConnection::Start(std::string host, int port, std::string nickname) {
	this->Connect(host, port);
	this->CmdUser(nickname, 8, nickname);
	this->CmdNick(nickname);
}

void IRCConnection::Start(std::string host, int port, std::string nickname,
			std::string username, std::string fullname) {
	this->Connect(host, port);
	this->CmdUser(username, 8, fullname);
	this->CmdNick(nickname);
}

void IRCConnection::Stop() {
	this->tcp.Disconnect();
}



void CmdJoin(std::string channel) {
	this->tcp.WriteLine("JOIN " + channel);
}

void CmdNick(std::string nick) {
	this->tcp.WriteLine("NICK " + nick);
}

void CmdPart(std::string channel) {
	this->tcp.WriteLine("PART " + channel);
}

void CmdPart(std::string channel, std::string reason) {
	this->tcp.WriteLine("PART " + channel + " " + reason);
}

void CmdUser(std::string username, std::string realname) {
	this->tcp.WriteLine("USER " + username + " 8 * :" + realname);
}
