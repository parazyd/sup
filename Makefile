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
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	chmod 4711 $(DESTDIR)$(PREFIX)/bin/$(BIN)
	mkdir -p $(DESTDIR)$(MANDIR)
	cp -r $(MAN) $(DESTDIR)$(MANDIR)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(PREFIX)/$(MAN)

.c.o:
	$(CC) $(CFLAGS) -c $<
