#
# (C) Copyleft 2011
# Late Lee from http://www.latelee.org
# 
# A simple Makefile for *ONE* project(c or/and cpp file) in *ONE*  directory
#
# note: 
# you can put head file(s) in 'include' directory, so it looks 
# a little neat.
#
# usage: $ make
#        $ make debug=y
###############################################################################

# cross compile...
CROSS_COMPILE = #arm_v5t_le-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR = $(CROSS_COMPILE)ar

ARFLAGS = cr
RM = -rm -rf
MAKE = make

CFLAGS = -Wall
debug = y

ifeq ($(debug), y)
CFLAGS += -g
else
CFLAGS += -O2 -s
endif

DEFS = 

CFLAGS += $(DEFS)

LDFLAGS = 

INCDIRS = -I./ -I./readline/

CFLAGS += $(INCDIRS)

LDFLAGS += 

# source file(s), including c file(s) cpp file(s)
# you can also use $(wildcard *.c), etc.
SRC_C   := $(wildcard *.c)
SRC_C   += $(wildcard readline/*.c)
SRC_CPP := $(wildcard *.cpp)

# object file(s)
OBJ_C   := $(patsubst %.c,%.o,$(SRC_C))
OBJ_CPP := $(patsubst %.cpp,%.o,$(SRC_CPP))


# executable file
target = a.out

###############################################################################

all: $(target)

$(target): $(OBJ_C) $(OBJ_CPP)
	@echo "Generating executable file..." $(notdir $(target))
	@$(CXX) $(CFLAGS) $^ -o $(target) $(LDFLAGS)

# make all .c or .cpp
%.o: %.c
	@echo "Compling: " $(addsuffix .c, $(basename $(notdir $@)))
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	@echo "Compling: " $(addsuffix .cpp, $(basename $(notdir $@)))
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@echo "cleaning..."
	@$(RM) $(target)
	@$(RM) *.o *.back *~

.PHONY: all clean
