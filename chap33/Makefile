CC=gcc
CFLAGS=-g -Wall -Werror -W -Wextra -pedantic -fsanitize=address
FILES=fclient fserv aserv cserv

all: $(FILES)

fclient: file-client.c
	$(CC) $(CFLAGS) -o fclient file-client.c

fserv: file-server.c
	$(CC) $(CFLAGS) -o fserv file-server.c

aserv: aio-server.c
	$(CC) $(CFLAGS) -o aserv aio-server.c

cserv: cache-server.c
	$(CC) $(CFLAGS) -o cserv cache-server.c

clean:
	rm -rf *.dSYM $(FILES)