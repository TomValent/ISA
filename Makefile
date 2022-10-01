CPP=g++
CFLAGS=-std=c++20 -Wall -pedantic -pie

feedreader: feedreader.o
	$(CPP) $(CFLAGS) feedreader.o -o feedreader

feedreader.o: main.cpp
	$(CPP) $(CFLAGS) -c main.cpp -o feedreader.o

clean:
	rm *.o feedreader -f

zip:
	zip xvalen27.zip *.cpp *.h Makefile