#include "socket.h"

void TCPClient::Connect(std::string host, int port) {
	char portstr[16];
	printf(portstr, "%d", port);

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// Don't care if it's IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream socket
	hints.ai_flags = AI_PASSIVE;		// Autofill IP

	struct addrinfo *servinfo;
	int status;
	if ((status = getaddrinfo(host.c_str(), portstr, &hints, &servinfo)))
		throw std::runtime_error(gai_strerror(status));

	if (!(this->socketFD = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)))
		throw std::runtime_error(strerror(errno));
	
	if (!(connect(this->socketFD, servinfo->ai_addr, servinfo->ai_addrlen)))
		throw std::runtime_error(strerror(errno));

	freeaddrinfo(servinfo);
}

void TCPClient::Disconnect() {
	close(this->socketFD);
}

int TCPClient::Write(std::string str) {
	int status;
	if (!(status = send(this->socketFD, str.c_str(), str.size(), 0)))
		throw std::runtime_error(strerror(errno));
	return status;
}

int TCPClient::WriteLine(std::string str) {
	str += "\r\n";
	return this->Write(str);
}

std::string TCPClient::Read(int len) {
	char *msg = new char[len];
	if (!(recv(this->socketFD, (void *)msg, len, 0)))
		throw std::runtime_error(strerror(errno));
	std::string str = std::string(msg);
	delete [] msg;
	return str;
}

std::string TCPClient::ReadLine() {
	std::string str;
	char chr;
	while (true) {
		if (!(recv(this->socketFD, (void *)&chr, 1, 0)))
			throw std::runtime_error(strerror(errno));
		if (chr == '\r') continue;
		if (chr == '\n') break;
		str += chr;
	}
	return str;
}

