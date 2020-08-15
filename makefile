all: vegada.buildrooms vegada.adventure

vegada.buildrooms: vegada.buildrooms.o
	gcc -g -Wall -std=c99 -o vegada.buildrooms vegada.buildrooms.o

vegada.adventure: vegada.adventure.o
	gcc -g -Wall -std=c99 -o vegada.adventure vegada.adventure.o

vegada.buildrooms.o: vegada.buildrooms.c vegada.buildrooms.h
	gcc -g -Wall -std=c99 -c vegada.buildrooms.c

vegada.adventure.o: vegada.adventure.c vegada.adventure.h
	gcc -g -Wall -std=c99 -c vegada.adventure.c

clean:
	rm vegada.buildrooms.o
	rm vegada.buildrooms
	rm vegada.adventure.o
	rm vegada.adventure
