all:	
	swig -c++ -python -o long_wrap.cpp long.i
	gcc -c -fPIC long_wrap.cpp -o long_wrap.o -I/usr/include/python2.7 -I../func -I ../class
	g++ -fPIC -shared long_wrap.o -I../func -I../class -L../func -L../class -llongClass -llong -o _longmodule.so


	
