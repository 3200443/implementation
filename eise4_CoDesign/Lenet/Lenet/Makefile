all: main

main.o: main.c
	gcc -Wall -g -c main.c -pg

fonctions_ES.o: fonctions_ES.c
	gcc -Wall -g -c fonctions_ES.c -pg

timers_b.o: timers_b.c
	gcc -Wall -g -c timers_b.c -pg

main: main.o fonctions_ES.o timers_b.o
	gcc -Wall -g main.o fonctions_ES.o timers_b.o -o main -lm -pg

test:
	./main

clean:
	rm -f *.o main