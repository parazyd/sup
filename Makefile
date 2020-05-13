# sup: scale user privileges
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

BIN = sup
MAN = $(BIN).1
OBJ = $(BIN:=.o) sha256.o

all: $(BIN)

$(OBJ): config.h config.mk

config.h:
	cp config.def.h config.h

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	rm -f $(BIN) $(OBJ)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANDIR)
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	cp -f $(MAN) $(DESTDIR)$(MANDIR)
	chmod 4711 $(DESTDIR)$(PREFIX)/bin/$(BIN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(MANDIR)/$(MAN)

.c.o:
	$(CC) $(CFLAGS) -c $<
