CXX=g++
CXXFLAGS=-std=c++11 -O3

LIB_FOLDER=lib
LIB_SRC=$(wildcard $(LIB_FOLDER)/*.c)
LIB_OBJS=$(notdir $(LIB_SRC:.c=.o))
LIB_NAME=libecc.a
ZIP_FILE=xmikus15.zip

all: bms1A bms1B

$(LIB_NAME): $(LIB_OBJS)
	ar -cvq $@ $^

%.o: $(LIB_FOLDER)/%.c
	$(CXX) $(CXXFLAGS) -c $<

encoder.o: Encoder.cpp Encoder.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
decoder.o: Decoder.cpp Decoder.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

bms1A: bms1A.cpp encoder.o $(LIB_NAME)
	$(CXX) $(CXXFLAGS) $^ -o $@
bms1B: bms1B.cpp decoder.o $(LIB_NAME)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf *.o bms1A bms1B $(LIB_NAME) $(ZIP_FILE)

zip:
	zip $(ZIP_FILE) Makefile ./$(LIB_FOLDER)/* *.cpp *.hpp
