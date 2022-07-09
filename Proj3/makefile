mytest: mqueue.o mytest.cpp
	g++ -Wall -g mytest.cpp mqueue.o -o mytest

mqueue.o: mqueue.cpp mqueue.h
	g++ -Wall -c -g mqueue.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *.o
	rm -rf mytest.dSYM
	rm mytest