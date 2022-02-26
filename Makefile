all: adventure
items.o: items.c items.h
	gcc -c -o items.o items.c
rooms.o: rooms.c rooms.h
	gcc -c -o rooms.o rooms.c
keywords.o: keywords.c keywords.h
	gcc -c -o keywords.o keywords.c
adventure.o: adventure.c
adventure: items.o rooms.o keywords.o adventure.o 
	gcc -o adventure items.o rooms.o keywords.o adventure.o

clean:
	rm -f *.o
	rm -f adventure