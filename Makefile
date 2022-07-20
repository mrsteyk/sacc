# sacc: saccomys gopher client
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

BIN = sacc
MAN = $(BIN).1
OBJ = $(BIN:=.o) ui_$(UI).o io_$(IO).o

GETVER = $$(git rev-parse --is-inside-work-tree >/dev/null 2>&1 \
	&& git describe --tags \
	|| echo $(DEFVERSION))

all: $(BIN)

config.h:
	cp config.def.h config.h

version.h: .git/refs/heads/
	printf '#define VERSION "%s"\n' "$(GETVER)" > $@

$(BIN): $(OBJ)
	$(CC) $(SACCLDFLAGS) -o $@ $(OBJ) $(IOLIBS) $(LIBS)

$(OBJ): config.mk common.h
sacc.o: config.h version.h
ui_ti.o: config.h
io_$(IO).o: io.h

clean:
	rm -f $(BIN) $(OBJ) version.h

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
             $(IOCFLAGS) \
             $(CFLAGS) \

SACCLDFLAGS = $(OSLDFLAGS) \
	      $(LDFLAGS) \

.git/refs/heads/:

.c.o:
	$(CC) $(SACCCFLAGS) -c $<
