NAME=libftprintf.a
CFLAGS=-Wall -Wextra -Werror -Iinclude -Ilibft/include
CC=gcc

SRCS=src/arg.c src/eval.c src/fmt.c src/fmt_f.c src/fmt_f_utils.c src/fmt_x.c \
	 src/fmt_alias.c src/fmt_dou.c src/fmt_utils.c src/utils.c src/parse.c \
	 src/printf.c src/va_printf.c src/write_buffer.c src/write_out.c

OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(OBJS): Makefile

$(NAME): $(OBJS)
	$(MAKE) -C libft
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
