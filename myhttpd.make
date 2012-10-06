all: myhttpd

myhttpd: myhttpd.o utility.o
	gcc -o myhttpd myhttpd.o utility.o

myhttpd.o: myhttpd.c
	gcc -c myhttpd.c -o myhttpd.o

utility.o: utility.c
	gcc -c utility.c -o utility.o

clean:
	rm *.o myhttpd
	clear
