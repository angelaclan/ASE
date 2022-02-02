CC=gcc
CFLAGS=-I.
DEPS = switchto.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)



switchto: switchto.o 
	$(CC) -o switchto switchto.o




clean:
	rm *.o switchto
