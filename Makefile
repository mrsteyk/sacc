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
	$(CC) $(SACCLDFLAGS) -o $@ $(OBJ) $(IOLIBS) $(LIBS)

$(OBJ): config.mk common.h
sacc.o: config.h
ui_ti.o: config.h
io_$(IO).o: io.h

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
SACCCFLAGS = $(OSCFLAGS) \
             -DVERSION=\"$(GETVER)\" \
             $(IOCFLAGS) \
             $(CFLAGS) \

SACCLDFLAGS = $(OSLDFLAGS) \
	      $(LDFLAGS) \

.c.o:
	$(CC) $(SACCCFLAGS) -c $<
