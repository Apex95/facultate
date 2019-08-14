.PHONY: build clean
build: so_scheduler.o thread_queue.o
	gcc -Wall -O3 -shared so_scheduler.o thread_queue.o -o libscheduler.so

so_scheduler.o: so_scheduler.h so_scheduler.c
	gcc -Wall -O3 -fPIC -c so_scheduler.c

thread_queue.o: thread_queue.h thread_queue.c
	gcc -Wall -O3 -fPIC -c thread_queue.c

clean:
	rm libscheduler.so
	rm so_scheduler.o
	rm thread_queue.o

