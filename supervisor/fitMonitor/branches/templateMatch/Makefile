#A makefile to compile a c program using openCV

CC = g++
LDFLAGS = -lm -g
CFLAGS  = -ggdb -O3 -Wall $(shell pkg-config opencv --cflags --libs)
SOURCES = $(wildcard f*.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = fitMonitor

all:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean : 
	@echo "Cleaning"
	@rm -f *.o *.txt $(EXECUTABLE)

