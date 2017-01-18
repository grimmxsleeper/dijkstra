all: dijkstra
dijkstra: dijkstra.c board.c func.c
	gcc dijkstra.c board.c func.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` -g -o dijkstra --std=gnu99

clean:
	rm -f dijkstra dijkstra.o
run: 
	./dijkstra
