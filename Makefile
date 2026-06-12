CC = gcc
CFLAGS = -Wall -Wextra -O3
TARGET = base

SRCS = base.c heuristicas.c
OBJS = $(SRCS:.c=.o)
HEADERS = heuristicas.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)