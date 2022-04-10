# sacc: saccomys gopher client
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

BIN = sacc
MAN = $(BIN).1
OBJ = $(BIN:=.o) ui_$(UI).o io_$(IO).o

all: $(BIN)

config.h:
	cp config.def.h config.h

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(IOLIBS) $(LIBS) -o $@

$(OBJ): config.h config.mk common.h io.h

clean:
	rm -f $(BIN) $(OBJ)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/
	chmod 555 $(DESTDIR)$(PREFIX)/bin/$(BIN)
	mkdir -p $(DESTDIR)$(MANDIR)
	sed -e "s/%VERSION%/$(GETVER)/" $(MAN) > $(DESTDIR)$(MANDIR)/$(MAN)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN) $(DESTDIR)$(MANDIR)/$(MAN)

# Stock FLAGS
SACCCFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700 -D_BSD_SOURCE -D_GNU_SOURCE \
             -DVERSION=\"$(GETVER)\" $(IOCFLAGS) $(CFLAGS)

.c.o:
	$(CC) $(SACCCFLAGS) -c $<
