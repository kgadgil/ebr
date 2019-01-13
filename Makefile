## BASIC MAKEFILE ##
# General syntax of a Makefile
# target [target ...] : [dependent ...]
#<tab>[command...]


CC = g++
CPPFLAGS = -std=c++11 -pthread -ggdb
#LDFLAGS

SOURCES = ebr.cpp

all: ebr

ebr: $(SOURCES)
	$(CC) $(CPPFLAGS) $(SOURCES) $(LDFLAGS) -o $@

#run: tut
	./ebr

clean:
	rm -f *.o ebr