.SUFFIXES: .c

PREFIX = src
INCPRE = include

SRCS = $(PREFIX)/main.c $(PREFIX)/image.c $(PREFIX)/html.c $(PREFIX)/util.c
OBJS = $(SRCS:.c=.o)
OUTPUT = album

CC = gcc
CFLAGS = -Wall -pedantic -I$(INCPRE) -g
LIBS = 

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS) $(LIBS)

$(PREFIX)/image.o: $(INCPRE)/image.h
$(PREFIX)/html.o: $(INCPRE)/html.h
$(PREFIX)/util.o: $(INCPRE)/util.h

test: $(OUTPUT)
	./$(OUTPUT) photos/*

clean:
	rm -f $(OBJS) $(OUTPUT) *.jpg *.html

depend:
	makedepend -I/usr/local/include/g++ -- $(CFLAGS) -- $(SRCS) 

# DO NOT DELETE
