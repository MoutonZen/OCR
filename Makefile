CC=gcc
CFLAGS=-W -Wall -Wextra -Werror -std=c99
LDFLAGS=-lm
EXEC= XOR
SRCS= main.c network.c
OBJS=$(SRCS:.c=.o)

all:

XOR: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS): $(SRCS)
	$(CC) -c $? $(CFLAGS)

clean:
	rm -rvf *~
	rm -rvf \#*
	rm -rvf $(OBJS)

fclean: clean
	rm -rvf $(EXEC)

re: fclean $(EXEC)
