CFLAGS=-Wall -g -O0 -DNDEBUG


CC = gcc
SRCDIR = src
OBJDIR = bin
SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJS    = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))


all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean: $(OBJDIR)
	rm -f $(OBJS)
