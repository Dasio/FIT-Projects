CXX=g++
CXXFLAGS=-std=c++11 -Wall -pedantic -pthread -g
SRCS=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
OBJS=$(subst .cpp,.o,$(SRCS))
INCLUDE=-I/usr/include/libxml2
LIBS=-lxml2
all: scanner spoof intercept massspoof chooser


scanner: scanner.cpp common.o Socket.o Packet.o Arp.o Scanner.o ICMPv6.o
	$(CXX) $(CXXFLAGS) $^ -o pds-scanner

spoof: spoof.cpp common.o Spoofer.o Packet.o Socket.o Arp.o ICMPv6.o
	$(CXX) $(CXXFLAGS) $^ -o pds-spoof

intercept: intercept.cpp common.o Packet.o Socket.o Interceptor.o
	$(CXX) $(CXXFLAGS) $^ -o pds-intercept $(LIBS)

massspoof: massspoof.cpp common.o Spoofer.o Packet.o Socket.o Arp.o ICMPv6.o XMLParser.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o pds-massspoof $(LIBS)

chooser: chooser.cpp common.o XMLParser.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o pds-chooser $(LIBS)


%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

clean:
	rm -rf *.o pds-scanner pds-spoof pds-intercept pds-massspoof pds-chooser

.PHONY: clean
