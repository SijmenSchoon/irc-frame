#ifndef SOCKET_H_GUARD
#define SOCKET_H_GUARD

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <iostream>
#include <stdexcept>

class TCPClient {
public:
	TCPClient();
	TCPClient(char ipv4[4], int port);
	TCPClient(std::string host, int port);

	void Connect(std::string host, int port);
	void Disconnect();

	int Write(std::string str);
	int WriteLine(std::string str);
	std::string Read(int len);
	std::string ReadLine();

private:
	int socketFD;
};

#endif
