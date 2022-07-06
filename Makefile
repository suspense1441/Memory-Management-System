# Hands-on experience for creating better memory management systems
# OS Assignment - 5
# 	Group - 43
# 	- Tushar Kishor Bokade 19CS30011
# 	- Aditya Vallakatla 19CS30051

all: libmemlab.a demo

libmemlab.a: lib_memlab.o
	ar rcs libmemlab.a lib_memlab.o

lib_memlab.o: memlab.cpp
	g++ -g -c memlab.cpp -o lib_memlab.o

demo: demo1.cpp demo2.cpp
	g++ demo1.cpp -L. -lmemlab -lpthread -o demo1
	g++ demo2.cpp -L. -lmemlab -lpthread -o demo2

clean: 
	rm demo1 demo2 lib_memlab.o libmemlab.a
