CXX=g++
CXXFLAGS=-std=c++11

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(notdir $(CPP_FILES:.cpp=.o))

bms2: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $< -c

clean:
	rm *.o

zip:
	zip xmikus15.zip *.cpp *.h Makefile

.PHONY: clean zip
