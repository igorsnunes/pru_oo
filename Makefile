CFLAGS+=-Wall -Werror -std=c++11 
LDLIBS+= -lpthread -lprussdrv -g
CC= g++
LIBS_PRU=/usr/local/lib
HEADERS_PRU=/usr/local/include


all: pru

pru:
	$(CC) -L$(LIBS_PRU) -I$(HEADERS_PRU)  $(CFLAGS) -c -fPIC -Wl,-rpath=$(LIBS_PRU) pru.cpp  -o pru.o $(LDLIBS)
	$(CC) -shared -Wl,-soname,libpruoo.so -o libpruoo.so pru.o -lc
install: pru
	cp libpruoo.so $(LIBS_PRU)
	cp pru.h $(HEADERS_PRU)
	chmod 0755 $(LIBS_PRU)/libpruoo.so
	ldconfig
clean:
	rm -f example *.o
main: 
	$(CC) -L$(LIBS_PRU) -I$(HEADERS_PRU)  $(CFLAGS) -Wl,-rpath=$(LIBS_PRU) pru.cpp main.cpp  -o main.o $(LDLIBS)
