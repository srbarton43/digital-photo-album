.SUFFIXES: .c

PREFIX = src

SRCS = $(PREFIX)/main.c
OBJS = $(SRCS:.c=.o)
OUTPUT = main

CC = gcc
CFLAGS = -Wall -pedantic
LIBS = 

main: $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS) $(LIBS)

run: $(OUTPUT)
	./$(OUTPUT)

clean:
	rm -f $(OBJS) $(OUTPUT)

depend:
	makedepend -I/usr/local/include/g++ -- $(CFLAGS) -- $(SRCS) 

# DO NOT DELETE
