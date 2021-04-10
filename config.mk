# Install paths
PREFIX = /usr/local
MANDIR = $(PREFIX)/share/man/man1

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

# Define NEED_ASPRINTF and/or NEED_STRCASESTR in your cflags if your system does
# not provide asprintf() or strcasestr(), respectively.
#CFLAGS = -DNEED_ASPRINTF -DNEED_STRCASESTR
