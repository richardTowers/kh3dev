#A makefile to cross compile a program for a Khepera 3 robot

CC = ~/Development/cross/bin/arm-angstrom-linux-gnueabi-gcc
#CC = gcc #For development...
CFLAGS = -Wall
LDFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = evoSlave

INCS = -I ~/Development/libraries/include
LIBS	= -L ~/Development/libraries/lib -lkorebot

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) $(INCS) $(LIBS) -o $(EXECUTABLE)

clean : 
	@echo "Cleaning"
	@rm -f *.o *.txt $(EXECUTABLE)

