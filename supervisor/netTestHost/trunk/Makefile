#A makefile to cross compile a program for a Khepera 3 robot

CC = ~/Development/cross/bin/arm-angstrom-linux-gnueabi-gcc
LDFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = netTestHost

INCS = -I ../libraries/include
LIBS	= -L ../libraries/lib -lkorebot

all: $(EXECUTABLE)
	@echo 'Target completed'

$(EXECUTABLE): $(OBJECTS)
	@echo 'Linking object files'

#Dependency rules for object files must go here:
#e.g.
#netTestHost.o: someFileIncludedInnetTestHost.o
#	@echo 'Compiling netTestHost.o'

clean : 
	@echo "Cleaning"
	@rm -f *.o $(EXECUTABLE)

