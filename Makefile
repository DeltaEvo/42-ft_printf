NAME=libftprintf.a
CFLAGS=-Wall -Wextra -Werror -Iinclude -Ilibft/include -g
CC=gcc

SRCS=src/arg.c src/eval.c src/fmt.c src/parse.c src/printf.c src/write_buffer.c src/write_out.c

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile

libft/libft.a:
	$(MAKE) -C libft

$(NAME): $(OBJS) libft/libft.a
	cp libft/libft.a $(NAME)
	ar rcs $(NAME) $(OBJS)

clean:
	$(MAKE) -C libft clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
