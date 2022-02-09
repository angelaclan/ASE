CC=gcc
CFLAGS=-I.
DEPS = yield.h list.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o yield.o
	$(CC) -o main main.o yield.o


clean:
	rm *.o yield main
