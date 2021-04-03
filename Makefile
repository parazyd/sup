# sup: scale user privileges
# See LICENSE file for copyright and license details.
.POSIX:

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# use system flags
SUP_CFLAGS   = ${CFLAGS} -Wall -Werror -pedantic -std=c99
SUP_CPPFLAGS = ${CPPFLAGS}
SUP_LDFLAGS  = ${LDFLAGS} -static -s

BIN = sup
MAN = $(BIN).1
OBJ = $(BIN:=.o) sha256.o

all: $(BIN)

$(OBJ): config.h

config.h:
	cp config.def.h config.h

.c.o:
	$(CC) -c $(SUP_CFLAGS) $(SUP_CPPFLAGS) $<

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(SUP_LDFLAGS) -o $@

clean:
	rm -f $(BIN) $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	cp -f $(MAN) $(DESTDIR)$(MANPREFIX)/man1
	chmod 4711 $(DESTDIR)$(PREFIX)/bin/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(MANPREFIX)/man1/$(MAN)

.PHONY: all clean install uninstall
