CPP=g++
CFLAGS=-std=c++20 -Wall -pedantic -pie

feedreader: arguments.o openssl.o
	$(CPP) $(CFLAGS) arguments.o openssl.o -o feedreader

arguments.o: Arguments.cpp Arguments.h
	$(CPP) $(CFLAGS) -c Arguments.cpp -o arguments.o

openssl.o: OpenSSL.cpp OpenSSL.h
	$(CPP) $(CFLAGS) -c OpenSSL.cpp -o openssl.o

clean:
	rm *.o feedreader -f

zip:
	zip xvalen27.zip *.cpp *.h Makefile