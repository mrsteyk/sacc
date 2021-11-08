#define clear(p)	do { void **_p = (void **)(p); free(*_p); *_p = NULL; } while (0);

typedef struct item Item;
typedef struct dir Dir;

enum {
	TXT,
	DIR,
	CSO,
	ERR,
	MAC,
	DOS,
	UUE,
	IND,
	TLN,
	BIN,
	MIR,
	IBM,
	GIF,
	IMG,
	URL,
	INF,
	UNK,
	BRK,
};

struct item {
	char type;
	char redtype;
	char *username;
	char *selector;
	char *host;
	char *port;
	char *raw;
	char *tag;
	void *dat;
	Item *entry;
};

struct dir {
	Item *items;
	size_t nitems;
	size_t printoff;
	size_t curline;
};

extern void (*diag)(char *, ...);

extern void die(const char *, ...);
extern size_t mbsprint(const char *, size_t);
#ifdef NEED_STRCASESTR
extern char *strcasestr(const char *, const char *);
#endif /* NEED_STRCASESTR */
extern const char *typedisplay(char);
extern int itemuri(Item *, char *, size_t);
extern void uicleanup(void);
extern void uidisplay(Item *);
extern char *uiprompt(char *, ...);
extern Item *uiselectitem(Item *);
extern void uisetup(void);
extern void uisigwinch(int);
extern void uistatus(char *, ...);
