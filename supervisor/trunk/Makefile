#A makefile to compile for the host computer

CC = g++
LDFLAGS = -lm -g
CFLAGS = -ggdb -O0 -Wall $(shell pkg-config opencv --cflags --libs)
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = supervisor

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean : 
	@echo "Cleaning"
	@rm -f *.o *.txt $(EXECUTABLE)

