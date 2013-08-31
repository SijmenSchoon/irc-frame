#!/usr/bin/make all
all:
	g++ src/irc.cpp src/socket.cpp src/main.cpp -o irc-frame -std=c++11
