build:
	gcc -Wall client/client.c client/message_handler.c client/db.c -o client.out
	gcc -Wall server/server.c server/db.c server/message_handler.c server/file.c -o server.out
clean:
	rm client.out server.out
run:
	./server.out 12345 users.conf shares.conf
	./client.out 127.0.0.1 12345
