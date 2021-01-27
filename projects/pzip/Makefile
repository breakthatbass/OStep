CC=gcc
CFLAGS=-pthread -O -g -Wall -Werror -W -Wextra -pedantic #-fsanitize=address 
SRC_FILES=pzip.c main.c
NAME = pzip

all: $(NAME)

$(NAME): $(SRC_FILES)
	$(CC) -o $(NAME) $(SRC_FILES) $(CFLAGS)

clean:
	rm $(NAME)
