CC = gcc
#CPPFLAGS = -DDEBUG -DLOG_LEVEL=LOG_DEBUG
CFLAGS = -Wall

.PHONY: all clean
build: vmsim.o list.o common_lin.o
	gcc -Wall -shared vmsim.o common_lin.o list.o -o libvmsim.so

common_lin.o: common_lin.c common.h util.h debug.h
	gcc -Wall -fPIC -c common_lin.c -o common_lin.o

vmsim.o: vmsim.c vmsim.h
	gcc -Wall -fPIC -c  vmsim.c -o vmsim.o

list.o: list.c list.h
	gcc -Wall -fPIC -c  list.c -o list.o	

clean:
	-rm -f *~ *.o
	rm libvmsim.so
