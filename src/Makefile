CC=gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -ggdb -lncurses
SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
all: fm 

fm: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o fm 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean: 
	rm -f *.o fm

.PHONY: all clean
