.PHONY: all clean fclean re

NAME = philo
OBJDIR = objects

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -pthread

SRCS = main.c \
		utils.c \
		thread.c \
		monitor_thread.c \
		fork_lock_two.c \
		utils_two.c \

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all