# Install paths
PREFIX = /usr/local
MANDIR = $(PREFIX)/share/man/man1

# Version to put in the ident string
VERSION = "1.06"
GETVER = $$(git rev-parse --is-inside-work-tree >/dev/null 2>&1 && \
	    git describe --tags || echo $(VERSION))

# UI type
# txt (textual)
#UI=txt
# ti (screen-oriented)
UI=ti
LIBS=-lcurses

# IO type
# clr (clear)
#IO = clr
# tls (Transport Layer Security)
IO = tls
IOLIBS = -ltls
IOCFLAGS = -DUSE_TLS

# OS compilation flags are used to expose the system interfaces
# Linux, OpenBSD
OSCFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700 -D_BSD_SOURCE -D_GNU_SOURCE
OSLDFLAGS = 
# FreeBSD
#OSCFLAGS = -I/usr/local/include
#OSLDFLAGS = -L/usr/local/lib

# Define NEED_ASPRINTF and/or NEED_STRCASESTR in your OS compilation flags
# if your system does not provide asprintf() or strcasestr(), respectively.
#OSCFLAGS = -DNEED_ASPRINTF -DNEED_STRCASESTR
