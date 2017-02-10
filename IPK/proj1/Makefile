CXX = g++49
FLAGS = -static-libstdc++ -std=c++11 -Wall -Wextra 
SFLAGS = -pthread

all: client server

client: client.cpp ClientArgParser.o Socket.o Packet.o Passwd.o
	$(CXX) $(FLAGS) client.cpp ClientArgParser.o Socket.o Packet.o Passwd.o -o $@

server: server.cpp Socket.o Packet.o Passwd.o
	$(CXX) $(FLAGS) $(SFLAGS) server.cpp Socket.o Packet.o Passwd.o -o $@

ClientArgParser.o: ClientArgParser.cpp ClientArgParser.h
	$(CXX) $(FLAGS) -c $<
Socket.o: Socket.cpp Socket.h
	$(CXX) $(FLAGS) -c $<
Packet.o: Packet.cpp Packet.h
	$(CXX) $(FLAGS) -c $<
Passwd.o: Passwd.cpp Passwd.h
	$(CXX) $(FLAGS) -c $<
zip:
	zip xmikus15.zip *.cpp *.h Makefile
clean:
	rm -f *.o client server
