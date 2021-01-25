CC=gcc
CFLAGS=-pthread -g
SRC_FILES=pzip.c
NAME = pzip

all: $(NAME)

$(NAME): $(SRC_FILES)
	$(CC) -o $(NAME) $(SRC_FILES) $(CFLAGS)

clean:
	rm $(NAME)