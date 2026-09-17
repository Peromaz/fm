CC=gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -ggdb -g
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
LINKS = -lncurses
all: fm 

fm: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o fm $(LINKS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean: 
	rm -f *.o fm

.PHONY: all clean
