CC = gcc -g -Wall --std=gnu99
LIBS = $(shell pkg-config --libs glib-2.0)
INCLUDE = $(shell pkg-config --cflags glib-2.0)

all: clean dijkstra
dijkstra:
	$(CC) dijkstra.c board.c func.c movegen.c $(INCLUDE) $(LIBS) -o dijkstra

clean:
	rm -f dijkstra

run:
	./dijkstra
