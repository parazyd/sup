CC?=gcc
DESTDIR?=
PREFIX?=/usr
VERSION=0.1
USER=root
GROUP=root
CFLAGS?=-O2 -Wall

all: config.h sup

config.h:
	cp config.def.h config.h

sup.o: config.h sup.c
	${CC} ${CFLAGS} -c sup.c

sup: sup.o
	${CC} ${LDFLAGS} sup.o -o sup

clean:
	rm -f sup.o sup

mrproper: clean
	rm -f config.h

install:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f sup ${DESTDIR}${PREFIX}/bin
	-chown ${USER}:${GROUP} ${DESTDIR}/${PREFIX}/bin/sup
	-chmod 4111 ${DESTDIR}${PREFIX}/bin/sup
	mkdir -p ${DESTDIR}${PREFIX}/share/man/man1
	sed s,VERSION,${VERSION}, sup.1 \
	  > ${DESTDIR}${PREFIX}/share/man/man1/sup.1
