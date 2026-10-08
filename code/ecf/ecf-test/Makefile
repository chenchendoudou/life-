# Makefile for CS:APP ECF test programs
# Usage:
#   make          - compile all .c files in this directory
#   make fork     - compile a specific program (e.g. fork)
#   make clean    - remove all generated executables

CC      = gcc
CFLAGS  = -Wall -O2 -g
INCLUDES = -I../../include
LDLIBS  = -lpthread -lm

# CS:APP helper library (provides Fork(), etc.)
CSAPP_SRC = ../../src/csapp.c

# Every .c file in this directory becomes an executable of the same name
SRCS  = $(wildcard *.c)
PROGS = $(SRCS:.c=)

all: $(PROGS)

$(PROGS): %: %.c $(CSAPP_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(PROGS)

.PHONY: all clean