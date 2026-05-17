CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread

NAME = codexion

SRC = main.c\
	arg_parse.c\
	coder.c\
	dongle.c\
	heap.c\
	helpers&time.c\
	logger.c\
	monitor.c\
	scheduler.c\
	simulator.c\

OBJS = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re