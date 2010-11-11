#A makefile to cross compile a program for a Khepera 3 robot

CC = ~/Development/cross/bin/arm-angstrom-linux-gnueabi-gcc
LDFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = netTestClient

INCS = -I ../libraries/include
LIBS	= -L ../libraries/lib -lkorebot

all: $(EXECUTABLE)
	@echo 'Target completed'

$(EXECUTABLE): $(OBJECTS)
	@echo 'Linking object files'

#Dependency rules for object files must go here:
#e.g.
#netTestClient.o: someFileIncludedInnetTestClient.o
#	@echo 'Compiling netTestClient.o'

clean : 
	@echo "Cleaning"
	@rm -f *.o $(EXECUTABLE)

