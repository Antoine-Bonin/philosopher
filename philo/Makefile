.PHONY: all clean fclean re

NAME = philo
OBJDIR = objects

CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -pthread
LDFLAGS = -pthread

SRCS = src/main.c \
		src/utils.c \
		src/get_status_death.c \
		src/routine.c \
		src/monitor.c \
		src/init_data.c \
		src/cleanup.c \
		src/fork.c \

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

all: $(NAME) includes/philo.h

$(NAME): $(OBJS) 
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all