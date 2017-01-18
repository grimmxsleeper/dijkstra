all: dijkstra
dijkstra: src/dijkstra.c src/board.c src/func.c
	gcc src/dijkstra.c src/board.c src/func.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -g -o dijkstra --std=gnu99

clean:
	rm -f dijkstra dijkstra.o
run:
	./dijkstra
