project: testcode.o files.o main.o
	cc files.o testcode.o main.o -o project
main.o: main.c testcode.h files.h
	cc -Wall -c main.c
testcode.o: testcode.c testcode.h files.h
	cc -Wall -c testcode.c
files.o: files.c files.h
	cc -Wall -c files.c
