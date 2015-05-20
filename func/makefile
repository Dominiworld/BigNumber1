all:	
	gcc -std=gnu99 -c -fPIC long.c -o long.o
	gcc -shared -o liblong.so long.o
	gcc -c main.c
	gcc main.o -L. -llong -o RES
	


	
