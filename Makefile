CC = gcc
CFLAGS = -D_POSIX_SOURCE -std=c99 -Wall -Os
INSTALL = /usr/bin/install
OBJ = nullidentdmod.o
TARGET = nullidentdmod
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

nullidentdmod: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm -f *.o $(TARGET)

install: all
	$(INSTALL) -D -s $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

.PHONY: all install clean
