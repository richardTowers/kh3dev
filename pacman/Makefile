#A makefile to compile for the host computer

CC = gcc

LDFLAGS = -lm
CFLAGS = -ggdb -O0 -Wall
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = pacman

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean : 
	@echo "Cleaning"
	@rm -f *.o *.txt $(EXECUTABLE)

