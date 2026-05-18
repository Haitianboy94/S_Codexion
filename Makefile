CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread

NAME = codexion

SRC = codexion.c\
	arg_parse.c\
	coder.c\
	dongle.c\
	heap.c\
	helpers_time.c\
	logger.c\
	monitor.c\
	scheduler.c\
	simulation.c\
	heap_utils.c\

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