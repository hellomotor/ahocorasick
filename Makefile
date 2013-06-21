
CC=gcc
CFLAGS=-Wall -fPIC -g -O2
INCLUDE=

_ahocorasick.so : py_wrapper.o aho-corasick.o slist.o
	$(CC) -shared -o $@ $? $(shell python-config --libs)

test : test.o aho-corasick.o slist.o
	$(CC) -o $@ $< $(shell python-config --libs) 

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(shell python-config --includes) 


clean:
	rm -f *.o *.so core.* test
