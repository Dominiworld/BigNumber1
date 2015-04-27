all:	
	gcc -std=gnu99 -c -fPIC 1.c -o 1.o
	gcc -shared -o -g libfirst.so 1.o
	g++ -c -fPIC 2.cpp -o 2.o
	g++ -shared -L. -lfirst -o libsecond.so 2.o
	g++ -c main.cpp
	g++ main.o -g -L. -lsecond -L. -lfirst -o RES
