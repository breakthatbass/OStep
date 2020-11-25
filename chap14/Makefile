CC=gcc
CFLAGS=-g -lm -std=c99 -pedantic
CMP_FILES=null malloc data free

null: null.c
		$(CC) -o null null.c $(CFLAGS) 

malloc: malloc.c
		$(CC) -o malloc malloc.c $(CFLAGS)

data: data.c
		$(CC) -o data data.c $(CFLAGS)

free: free.c
		$(CC) -o free free.c $(CFLAGS)

clean:
		rm $(CMP_FILES)
		rm -rf *.dSYM
