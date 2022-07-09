mytest: swarm.o mytest.cpp
	g++ -Wall -g mytest.cpp swarm.o -o mytest

swarm.o: swarm.cpp swarm.h
	g++ -Wall -c -g swarm.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *.o
	rm -rf mytest.dSYM
	rm mytest