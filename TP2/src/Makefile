CC=gcc
CFLAGS=-I.
DEPS = switchto.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o switchto.o
	$(CC) -o main main.o switchto.o


clean:
	rm *.o switchto main
