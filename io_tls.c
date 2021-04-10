#include <string.h>
#include <unistd.h>
#include <netdb.h>

#include <sys/socket.h>

#include <tls.h>

#include "common.h"
#include "io.h"

int tls;

static int
close_tls(struct cnx *c)
{
	int r;

	if (tls) {
		if (c->tls) {
			do {
				r = tls_close(c->tls);
			} while (r == TLS_WANT_POLLIN || r == TLS_WANT_POLLOUT);
		}
		tls_free(c->tls);
	}

	return close(c->sock);
}

static int
connect_tls(struct cnx *c, struct addrinfo *ai, const char *host)
{
	struct tls *t;
	int s;

	c->tls = NULL;
	s = c->sock;

	if (connect(s, ai->ai_addr, ai->ai_addrlen) == -1)
		return -1;

	if (tls) {
		if ((t = tls_client()) == NULL)
			return -1;
		if (tls_connect_socket(t, s, host) == -1)
			return -1;

		c->tls = t;
	}

	return 0;
}

static void
connerr_tls(struct cnx *c, const char *host, const char *port, int err)
{
	if (c->sock == -1) {
		diag("Can't open socket: %s", strerror(err));
	} else {
		if (tls && c->tls) {
			diag("Can't establish TLS with \"%s\": %s",
			     host, tls_error(c->tls));
		} else {
			diag("Can't connect to: %s:%s: %s", host, port,
			     strerror(err));
		}
	}
}

static char *
parseurl_tls(char *url)
{
	char *p;

	if (p = strstr(url, "://")) {
		if (!strncmp(url, "gopher", p - url)) {
			if (tls)
				diag("Switching from gophers to gopher");
			tls = 0;
		} else if (!strncmp(url, "gophers", p - url)) {
			tls = 1;
		} else {
			die("Protocol not supported: %.*s", p - url, url);
		}
		url = p + 3;
	}

	return url;
}

static ssize_t
read_tls(struct cnx *c, void *buf, size_t bs)
{
	ssize_t n;

	if (tls && c->tls) {
		do {
			n = tls_read(c->tls, buf, bs);
		} while (n == TLS_WANT_POLLIN || n == TLS_WANT_POLLOUT);
	} else {
		n = read(c->sock, buf, bs);
	}

	return n;
}

static ssize_t
write_tls(struct cnx *c, void *buf, size_t bs)
{
	ssize_t n;

	if (tls) {
		do {
			n = tls_write(c->tls, buf, bs);
		} while (n == TLS_WANT_POLLIN || n == TLS_WANT_POLLOUT);
	} else {
		n = write(c->sock, buf, bs);
	}

	return n;
}

int (*ioclose)(struct cnx *) = close_tls;
int (*ioconnect)(struct cnx *, struct addrinfo *, const char *) = connect_tls;
void (*ioconnerr)(struct cnx *, const char *, const char *, int) = connerr_tls;
char *(*ioparseurl)(char *) = parseurl_tls;
ssize_t (*ioread)(struct cnx *, void *, size_t) = read_tls;
ssize_t (*iowrite)(struct cnx *, void *, size_t) = write_tls;
