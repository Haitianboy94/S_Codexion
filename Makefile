CC		= cc
CFLAGS	= -Wall -Werror -Wextra -pthread
NAME	= codexion

SRC		= codexion.c\
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
		coder_helper.c\
		simulation_helper.c\

OBJS	= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) $(NAME)_san

re: fclean all

san: fclean
	$(CC) $(CFLAGS) -fsanitize=thread $(SRC) -o $(NAME)_san

run: $(NAME)
	-@./$(NAME) 1 800 200 200 200 3 50 fifo
	-@./$(NAME) 5 800 200 200 200 3 50 fifo
	-@./$(NAME) 5 800 200 200 200 3 50 edf
	-@./$(NAME) 5 200 800 200 200 3 50 fifo
	-@./$(NAME) 2 800 200 200 200 3 50 fifo

.PHONY: all clean fclean re run san