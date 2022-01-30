CC=gcc
CFLAGS=-I.
DEPS = try.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


try_mul: try_mul.o try.o 
	$(CC) -o try_mul try_mul.o try.o 

display_stack: display_stack.o 
	$(CC) -o display_stack display_stack.o 



clean:
	rm *.o try_mul display_stack