CC = clang
CFLAGS = -Wall -Wextra -Werror -pedantic
LDFLAGS = -lm
OBJS = hello.o

.PHONY: clean format

all: hello

hello: $(OBJS)
	$(CC) $(LDFLAGS) -o hello $(OBJS)

clean:
	rm -f hello $(OBJS)

format:
	clang-format -i -style=file *.[ch]

