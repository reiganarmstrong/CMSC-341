mytest: hist.o mytest.cpp
	g++ -Wall -g mytest.cpp hist.o -o mytest

hist.o: hist.cpp hist.h
	g++ -Wall -c hist.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *.o*
	rm mytest