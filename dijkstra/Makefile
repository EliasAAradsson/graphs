CC = gcc
CFLAGS = -Wall -Wextra -std=c2x
TARGET = dijkstra.out
OBJS = heap.o main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

main.o: main.c heap.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
