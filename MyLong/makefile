all:	
	gcc -std=gnu99 -c -fPIC long.c -o long.o
	gcc -shared -o liblong.so long.o
	g++ -c -fPIC longClass.cpp -o longClass.o
	g++ -shared -L. -llong -o liblongClass.so longClass.o
	swig -c++ -python -o long_wrap.cpp long.i
	gcc -c -fPIC long_wrap.cpp -o long_wrap.o -I/usr/include/python2.7
	g++ -fPIC -shared long_wrap.o -L. -llongClass -llong -o _longmodule.so


	
