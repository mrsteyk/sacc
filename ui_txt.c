#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "common.h"

int lines;

static int
termlines(void)
{
	struct winsize ws;

	if (ioctl(1, TIOCGWINSZ, &ws) < 0) {
		die("Could not get terminal resolution: %s",
		    strerror(errno));
	}

	return ws.ws_row-1; /* one off for status bar */
}

void
uisetup(void) {
	lines = termlines();
}

void
uicleanup(void) {
	return;
}

void
help(void)
{
	puts("Commands:\n"
	     "N = [1-9]...: browse item N.\n"
	     "0: browse previous item.\n"
	     "n: show next page.\n"
	     "p: show previous page.\n"
	     "t: go to the top of the page\n"
	     "b: go to the bottom of the page\n"
	     "!: refetch failed item.\n"
	     "^D, q: quit.\n"
	     "h, ?: this help.");
}

static int
ndigits(size_t n)
{
	return (n < 10) ? 1 : (n < 100) ? 2 : 3;
}

static void
printstatus(Item *item, char c)
{
	Dir *dir = item->dat;
	size_t nitems = dir ? dir->nitems : 0;
	unsigned long long printoff = dir ? dir->printoff : 0;

	printf("%3lld%%%*c %s:%s%s [%c]: ",
	       (printoff + lines >= nitems) ? 100 :
	       (printoff + lines) * 100 / nitems, ndigits(nitems)+2, '|',
	       item->host, item->port, item->selector, c);
}

char *
uiprompt(char *s)
{
	char *input = NULL;
	size_t n = 0;

	fputs(s, stdout);
	fflush(stdout);

	if (getline(&input, &n, stdin) > 1)
		return input;

	free(input);
	return NULL;
}

void
display(Item *entry)
{
	Item **items;
	Dir *dir = entry->dat;
	size_t i, lines, nitems;
	int nd;

	if (!(entry->type == '1' || entry->type == '7') || !dir)
		return;

	items = dir->items;
	nitems = dir->nitems;
	lines = dir->printoff + termlines();
	nd = ndigits(nitems);

	for (i = dir->printoff; i < nitems && i < lines; ++i) {
		printf("%*zu %s %s\n",
		       nd, i+1, typedisplay(items[i]->type), items[i]->username);
	}

	fflush(stdout);
}

Item *
selectitem(Item *entry)
{
	Dir *dir = entry->dat;
	static char c;
	char buf[BUFSIZ], nl;
	int item, nitems, lines;

	nitems = dir ? dir->nitems : 0;
	if (!c)
		c = 'h';

	do {
		item = -1;
		printstatus(entry, c);
		fflush(stdout);

		if (!fgets(buf, sizeof(buf), stdin)) {
			putchar('\n');
			return NULL;
		}
		if (isdigit(*buf))
			c = '\0';
		else if (!strcmp(buf+1, "\n"))
			c = *buf;

		switch (c) {
		case '\0':
			break;
		case 'q':
			return NULL;
		case 'n':
			lines = termlines();
			if (lines < nitems - dir->printoff &&
			    lines < (size_t)-1 - dir->printoff)
				dir->printoff += lines;
			return entry;
		case 'p':
			lines = termlines();
			if (lines <= dir->printoff)
				dir->printoff -= lines;
			else
				dir->printoff = 0;
			return entry;
		case 'b':
			lines = termlines();
			if (nitems > lines)
				dir->printoff = nitems - lines;
			else
				dir->printoff = 0;
			return entry;
		case 't':
			dir->printoff = 0;
			return entry;
		case '!':
			if (entry->raw)
				continue;
			return entry;
		case 'h':
		case '?':
			help();
			continue;
		default:
			c = 'h';
			continue;
		}

		if (*buf < '0' || *buf > '9')
			continue;

		nl = '\0';
		if (sscanf(buf, "%d%c", &item, &nl) != 2 || nl != '\n')
			item = -1;
	} while (item < 0 || item > nitems);

	if (item > 0)
		return dir->items[item-1];

	return entry->entry;
}
