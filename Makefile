XMLLDFLAGS!=pkg-config --libs libxml-2.0
XMLLDFLAGS?=$(shell pkg-config --libs libxml-2.0)
XMLCFLAGS!=pkg-config --cflags libxml-2.0
XMLCFLAGS?=$(shell pkg-config --cflags libxml-2.0)

CPP=g++
CFLAGS=-std=c++17 -Wall -pedantic -fPIC -static-libstdc++ $(XMLCFLAGS)
LDFLAGS = -L/usr/local/ssl/lib $(XMLLDFLAGS)
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
	rm *.o feedreader xvalen27.tar -f

tar:
	tar -cf xvalen27.tar *.cpp *.h Makefile README.md

test:
	echo "No tests"