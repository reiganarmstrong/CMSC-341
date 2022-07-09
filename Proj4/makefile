mytest: hash.o file.o mytest.cpp
	g++ -Wall -g mytest.cpp file.o hash.o -o mytest

tempTest: file.o mytest.cpp
	g++ -Wall -g tempTest.cpp file.o -o tempTest

hash.o: hash.cpp hash.h
	g++ -Wall -c -g hash.cpp

file.o: file.cpp file.h
	g++ -Wall -c -g file.cpp

run:
	./mytest

runTemp:
	./tempTest

val:
	valgrind ./mytest

clean:
	rm *.o mytest
	rm -rf mytest.dSYM
cleanTemp:
	rm *.o tempTest
	rm -rf tempTest.dSYM