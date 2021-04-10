#include <netdb.h>

struct cnx {
#ifdef USE_TLS
	struct tls *tls;
#endif
	int sock;
};

extern int tls;

extern int (*ioclose)(struct cnx *);
extern int (*ioconnect)(struct cnx *, struct addrinfo *, const char *);
extern void (*ioconnerr)(struct cnx *, const char *, const char *, int);
extern char *(*ioparseurl)(char *);
extern ssize_t (*ioread)(struct cnx *, void *, size_t);
extern ssize_t (*iowrite)(struct cnx *, void *, size_t);
