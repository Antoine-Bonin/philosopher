.PHONY: all clean fclean re

NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread

SRCS = main.c \
		utils.c \
		thread.c \
		monitor_thread.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all