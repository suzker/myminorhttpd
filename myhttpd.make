all: myhttpd

myhttpd: myhttpd.o utility.o scheduler.o queue.o heap.o
	gcc -o myhttpd myhttpd.o utility.o heap.o queue.o scheduler.o -lpthread

myhttpd.o: myhttpd.c
	gcc -c myhttpd.c -o myhttpd.o

utility.o: utility.c
	gcc -c utility.c -o utility.o

scheduler.o: scheduler.c heap.o queue.o
	gcc -c scheduler.c -o scheduler.o

heap.o: heap.c
	gcc -c heap.c -o heap.o

queue.o: queue.c
	gcc -c queue.c -o queue.o

clean:
	rm *.o myhttpd
	clear
