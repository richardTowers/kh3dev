#A makefile to cross compile a program for a Khepera 3 robot

CC = ../cross/bin/arm-angstrom-linux-gnueabi-gcc
#CFLAGS = -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = ffnn

INCS = -I ../libraries/include
LIBS	= -L ../libraries/lib -lkorebot

all: $(EXECUTABLE)
	@echo 'Target completed'

$(EXECUTABLE): $(OBJECTS)
	@echo 'Linking object files'

#Dependency rules for object files must go here:
#e.g.
#ffnn.o: someFileIncludedInffnn.o
#	@echo 'Compiling ffnn.o'

clean : 
	@echo "Cleaning"
	@rm -f *.o $(EXECUTABLE)

