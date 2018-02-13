NAME=libftprintf.a
CFLAGS=-Iinclude
CC ?= gcc

SRCS=src/arg.c src/eval.c src/fmt.c src/parse.c src/printf.c src/write.c

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
