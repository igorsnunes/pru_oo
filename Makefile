CFLAGS+=-Wall -Werror -std=c++11 
LDLIBS+= -lpthread -lprussdrv -g
CC= g++
LIBS_PRU=/usr/local/lib
HEADERS_PRU=/usr/local/include


all: pru

pru:
	$(CC) -L$(LIBS_PRU) -I$(HEADERS_PRU)  $(CFLAGS) -c -fPIC -Wl,-rpath=$(LIBS_PRU) pru.cpp  -o pru.o $(LDLIBS)
install: pru
	$(CC) -shared -Wl,-soname,libpruoo.so -o libpruoo.so pru.o -lc
	cp libpruoo.so $(LIBS_PRU)
	cp pru.h $(HEADERS_PRU)
clean:
	rm -f example *.o
main: 
	$(CC) -L$(LIBS_PRU) -I$(HEADERS_PRU)  $(CFLAGS) -Wl,-rpath=$(LIBS_PRU) pru.cpp main.cpp  -o main.o $(LDLIBS)
