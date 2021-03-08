CFLAGS = -Wall

xmod: main.o aux.o logs.o
	gcc -o xmod main.o aux.o logs.o

main.o: main.c aux.h logs.h
	gcc $(CFLAGS) -c main.c

aux.o: aux.c aux.h
	gcc $(CFLAGS) -c aux.c

logs.o: logs.c logs.h
	gcc $(CFLAGS) -c logs.c

clean:
	rm *o xmod
