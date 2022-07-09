mytest: graph.o mytest.cpp
	g++ -Wall -g mytest.cpp graph.o -o mytest

graph.o: graph.cpp graph.h
	g++ -Wall -c -g graph.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *.o
	rm -rf mytest.dSYM
	rm mytest