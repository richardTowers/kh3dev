#A makefile to compile for the host computer

CC = gcc #For development...
LDFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = evoMaster

all:
	$(CC) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean : 
	@echo "Cleaning"
	@rm -f *.o *.txt $(EXECUTABLE)

