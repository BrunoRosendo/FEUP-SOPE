CFLAGS = -Wall

xmod: main.o parsing.o
	gcc -o xmod main.o parsing.o

main.o: main.c parsing.h
	gcc $(CFLAGS) -c main.c

parsing.o: parsing.c parsing.h
	gcc $(CFLAGS) -c parsing.c

clean:
	rm *o xmod
