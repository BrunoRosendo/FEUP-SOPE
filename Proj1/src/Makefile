CFLAGS = -Wall

xmod: main.o aux.o
	gcc -o xmod main.o aux.o

main.o: main.c aux.h
	gcc $(CFLAGS) -c main.c

aux.o: aux.c aux.h
	gcc $(CFLAGS) -c aux.c

clean:
	rm *o xmod
