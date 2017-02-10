CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -pthread -g
LINKS = -lssh
SRCS = $(wildcard *.cpp)
BIN = fakesrv
OBJS=$(subst .cpp,.o,$(SRCS))
ARCHIVE_NAME = xmikus15.tar.gz

server: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN) $(LINKS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<
archive:
	tar -cvzf $(ARCHIVE_NAME) *.cpp *.h Makefile README manual.pdf
test:
	 ./fakesrv -m ftp -a 127.0.0.1 -p 1234 -l test.log
test2:
	./fakesrv -m ssh -a 127.0.0.1 -p 1234 -l test.log -r ./my.key
clean:
	rm -f *.o $(BIN) $(ARCHIVE_NAME)
.PHONY: archive clean

