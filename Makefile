CPP=g++
CFLAGS=-std=c++17 -Wall -pedantic -pie -fPIC -static-libstdc++
LDFLAGS = -L/usr/local/ssl/lib
LDLIBS = -lssl -lcrypto

feedreader: arguments.o openssl.o parser.o
	$(CPP) $(CFLAGS) arguments.o openssl.o parser.o -o feedreader $(LDFLAGS) $(LDLIBS)

arguments.o: Arguments.cpp Arguments.h
	$(CPP) $(CFLAGS) -c Arguments.cpp -o arguments.o

openssl.o: OpenSSL.cpp OpenSSL.h
	$(CPP) $(CFLAGS) -c OpenSSL.cpp -o openssl.o

parser.o: Parser.cpp Parser.h
	$(CPP) $(CFLAGS) -c Parser.cpp -o parser.o

clean:
	rm *.o feedreader -f xvalen27.zip

zip:
	zip xvalen27.zip *.cpp *.h Makefile