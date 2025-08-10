.PHONY: all clean fclean re

NAME = philo
OBJDIR = objects

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 
LDFLAGS = -pthread 

SRCS = src/main.c \
		src/utils.c \
		src/utils_two.c \
		src/routine.c \
		src/monitor.c \
		src/init_data.c \
		src/free.c \
		src/fork.c \

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all