NAME=libftprintf.a
CFLAGS=-Iinclude -Ilibft/include
CC=gcc

SRCS=src/arg.c src/eval.c src/fmt.c src/parse.c src/printf.c src/write_buffer.c src/write_out.c

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile

$(NAME): $(OBJS)
	cp libft/libft.a $(NAME)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
