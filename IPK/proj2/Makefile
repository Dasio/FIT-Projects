CXX = g++
FLAGS = -std=c++11 -Wall -Wextra -pedantic -pthread -g
SBOOST = -lboost_system -lboost_program_options -lboost_filesystem -lboost_date_time
CBOOST = -lboost_system
SERVER_BIN = ipkhttpserver
CLIENT_BIN = ipkhttpclient
all: install bins

bins: server client
server: reply.o request_parser.o connection_manager.o connection.o common.o main.o server.o
	$(CXX) $(FLAGS) $^ -o $(SERVER_BIN) $(SBOOST)
client: common.o uriparser.o client.o
	$(CXX) $(FLAGS) $^ -o $(CLIENT_BIN) $(CBOOST)
main.o: main.cpp
	$(CXX) $(FLAGS) $< -c
%.o: %.cpp %.h
	$(CXX) $(FLAGS) $< -c

.PHONY: clean cleanlogs cleanfiles pack test install
install:
	echo "ipk2015" | sudo -S apt-get -y install libboost-all-dev
	sudo apt-get -y install apache2-utils
test:
	bash ./tests.sh
pack:
	tar -cvzf xmikus15.tgz *.cpp *.h Makefile tests.sh www README
cleanlogs:
	rm -f *.header *.payload *.log
cleanfiles:
	rm -f *.o $(SERVER_BIN) $(CLIENT_BIN) xmikus15.tgz
clean: cleanlogs cleanfiles
