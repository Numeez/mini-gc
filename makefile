CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main

SRC = main.c \
      snekobject.c \
      stack.c \
      vm.c \
      sneknew.c \
      mem.c \
      ./munit/munit.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
