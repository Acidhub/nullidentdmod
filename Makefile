CC = gcc
CFLAGS = -D_POSIX_SOURCE -std=c99 -Wall -Os
INSTALL = /usr/bin/install
OBJ = nullidentdmod.o
TARGET = nullidentdmod
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
UNITDIR= /usr/lib/systemd/system

all: $(TARGET)

nullidentdmod: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm -f *.o $(TARGET)

install: all
	$(INSTALL) -D -s $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)
	$(INSTALL) -Dm644 $(TARGET).socket $(DESTDIR)$(UNITDIR)/$(TARGET).socket
	$(INSTALL) -Dm644 $(TARGET)@.service $(DESTDIR)$(UNITDIR)/$(TARGET)@.service

.PHONY: all install clean
