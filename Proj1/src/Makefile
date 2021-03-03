CFLAGS = -Wall

xmod: main.o
	gcc -o xmod main.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	rm *o xmod
