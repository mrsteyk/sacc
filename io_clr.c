#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>

#include "common.h"
#include "io.h"

static int
close_clr(struct cnx *c)
{
	return close(c->sock);
}

static int
connect_clr(struct cnx *c, struct addrinfo *ai, const char *host)
{
	return connect(c->sock, ai->ai_addr, ai->ai_addrlen);
}

static void
connerr_clr(struct cnx *c, const char *host, const char *port, int err)
{
	if (c->sock == -1)
		diag("Can't open socket: %s", strerror(err));
	else
		diag("Can't connect to: %s:%s: %s", host, port, strerror(err));
}

static char *
parseurl_clr(char *url)
{
	char *p;

	if (p = strstr(url, "://")) {
		if (strncmp(url, "gopher", p - url))
			die("Protocol not supported: %.*s", p - url, url);

		url = p + 3;
	}

	return url;
}

static ssize_t
read_clr(struct cnx *c, void *buf, size_t bs)
{
	return read(c->sock, buf, bs);
}

static ssize_t
write_clr(struct cnx *c, void *buf, size_t bs)
{
	return write(c->sock, buf, bs);
}

int (*ioclose)(struct cnx *) = close_clr;
int (*ioconnect)(struct cnx *, struct addrinfo *, const char *) = connect_clr;
void (*ioconnerr)(struct cnx *, const char *, const char *, int) = connerr_clr;
char *(*ioparseurl)(char *) = parseurl_clr;
ssize_t (*ioread)(struct cnx *, void *, size_t) = read_clr;
ssize_t (*iowrite)(struct cnx *, void *, size_t) = write_clr;
