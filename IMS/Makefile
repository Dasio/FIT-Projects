DOC             = documentation
PDF             = $(DOC).pdf
TAR             = 04_xmikus15_xhosta03.tar.gz
SOURCES         = CA.cpp Cell.cpp ArgParser.cpp main.cpp
HEADERS	= CA.hpp Cell.hpp ArgParser.hpp bitmap_image.hpp
BIN             = CA

CXX             = g++
LIBS            =
CXXFLAGS        = -g -std=c++11 -Wall -Wextra -pedantic


###########################################

all:	$(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(BIN) $(LIBS)

run:
	./CA -l 1000 -s 30 -f run -d statistics.log

tex:
	# Double-pass compiling
	pdflatex $(DOC).tex
	pdflatex $(DOC).tex

rebuild: clean all

###########################################

clean:
	rm -rf core* *~ $(BIN).o $(BIN) *.log $(TAR) *.txt *.bmp \
	$(DOC).dvi $(DOC).aux $(DOC).toc $(DOC).synctex.gz $(DOC).out

tar: clean tex all
	tar -cvzf $(TAR) $(SOURCES) $(HEADERS) Makefile $(PDF)
