CC = gcc
CFLAGS = -Wall

build: main.o libhash.so
	$(CC) -std=c89 $(CFLAGS) -o tema1 $< -l :libhash.so -L.
main.o: main.c main.h
	$(CC) -std=c89 $(CFLAGS) -c $< -o $@
    
clean:
	rm -rf *.o tema1
    
run:
	valgrind --show-leak-kinds=all --track-origins=yes --leak-check=full --log-file=./log.txt ./tema1 ab <./test.in >./test.out
