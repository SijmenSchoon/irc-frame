#include "socket.h"

TCPClient::TCPClient() { }

void TCPClient::Connect(std::string host, int port) {
#ifdef DEBUG
	printf("[DEBUG] Opening TCPClient to %s:%d\n", host.c_str(), port);
#endif
	char portstr[16];
	sprintf(portstr, "%d", port);

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// Don't care if it's IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream socket
	hints.ai_flags = AI_PASSIVE;		// Autofill IP

	struct addrinfo *servinfo;
	int status;
#ifdef DEBUG
	printf("[DEBUG] Getting address info\n");
#endif
	if ((status = getaddrinfo(host.c_str(), portstr, &hints, &servinfo)) != 0)
		throw std::runtime_error(gai_strerror(status));

#ifdef DEBUG
	printf("[DEBUG] Getting socket descriptor\n");
#endif
	if ((this->socketFD = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0)
		throw std::runtime_error(strerror(errno));
	
#ifdef DEBUG
	printf("[DEBUG] Connecting to socket\n");
#endif
	if ((connect(this->socketFD, servinfo->ai_addr, servinfo->ai_addrlen)) < 0)
		throw std::runtime_error(strerror(errno));

	freeaddrinfo(servinfo);
#ifdef DEBUG
	printf("[DEBUG] Connected successfully!\n");
#endif
}

void TCPClient::Disconnect() {
	close(this->socketFD);
}

int TCPClient::Write(std::string str) {
	int status;
#ifdef DEBUG
	printf("[DEBUG] Writing data: %s", str.c_str());
#endif
	if (!(status = send(this->socketFD, str.c_str(), str.size(), 0)))
		throw std::runtime_error(strerror(errno));
	return status;
}

int TCPClient::WriteLine(std::string str) {
	str += "\r\n";
	return this->Write(str);
}

std::string TCPClient::Read(int len) {
#ifdef DEBUG
	printf("[DEBUG] Reading data...\n");
#endif
	char *msg = new char[len];
	if (!(recv(this->socketFD, (void *)msg, len, 0)))
		throw std::runtime_error(strerror(errno));
	std::string str = std::string(msg);
	delete [] msg;
	return str;
}

std::string TCPClient::ReadLine() {
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(this->socketFD, &rfds);
	
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	
	int retval = select(this->socketFD + 1, &rfds, NULL, NULL, &tv);
	
	if (retval == -1) printf("Select failed\n");
	else if (retval) {
		
		std::string str;
		char chr;
		while (true) {
			if (!(recv(this->socketFD, (void *)&chr, 1, 0)))
				throw std::runtime_error(strerror(errno));
			if (chr == '\r') continue;
			if (chr == '\n') break;
			str += chr;
		}
#ifdef DEBUG
		printf("[DEBUG] Read data line: %s...\n", str.c_str());
#endif
		return str;
	}
	
	return std::string();
}

