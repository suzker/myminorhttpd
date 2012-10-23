all: myhttpd

myhttpd: myhttpd.o utility.o scheduler.o queue.o heap.o tpool.o network.o server.o
	gcc -o myhttpd myhttpd.o utility.o heap.o queue.o scheduler.o tpool.o network.o server.o -lpthread

myhttpd.o: myhttpd.c
	gcc -c myhttpd.c -o myhttpd.o

utility.o: utility.c
	gcc -c utility.c -o utility.o

scheduler.o: scheduler.c
	gcc -c scheduler.c -o scheduler.o

tpool.o: tpool.c
	gcc -c tpool.c -o tpool.o

heap.o: heap.c
	gcc -c heap.c -o heap.o

queue.o: queue.c
	gcc -c queue.c -o queue.o

network.o: network.c
	gcc -c network.c -o network.o

server.o: server.c
	gcc -c server.c -o server.o

clean:
	rm *.o myhttpd
	clear
