CC=gcc
CFLAGS=-g -Wall -Werror -W -Wextra -pedantic -fsanitize=address

all: fclient fserv

fclient: file-client.c
	$(CC) $(CFLAGS) -o fclient file-client.c

fserv: file-server.c
	$(CC) $(CFLAGS) -o fserv file-server.c

clean:
	rm -rf *.dSYM fclient fserv