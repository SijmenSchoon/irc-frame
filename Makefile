#!/usr/bin/make -f
all:
	g++ src/irc.cpp src/socket.cpp src/main.cpp -o irc-frame -std=c++11
