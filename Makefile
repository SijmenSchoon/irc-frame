#!/usr/bin/make -f

LIBS=-lboost_regex
SOURCES=src/irc.cpp src/socket.cpp src/main.cpp
OUTPUT=irc-frame
CFLAGS=-std=c++11 -pthread

all:
	g++ $(SOURCES) -o $(OUTPUT) $(CFLAGS) $(LIBS)
	
dbg:
	g++ -g -DDEBUG $(SOURCES) -o $(OUTPUT) $(CFLAGS) $(LIBS)
	
