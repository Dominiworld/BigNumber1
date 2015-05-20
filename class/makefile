all:	
	g++ -c -fPIC longClass.cpp -o longClass.o -I../func
	g++ -shared -L../func -lfirst -o liblongClass.so longClass.o
	g++ -c main.cpp -I../1
	g++ main.o -g -L. -llongClass -L../func -llong -o RES
