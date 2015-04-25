CFLAGS+=-Wall -Werror -std=c++11 
LDLIBS+= -lpthread -lprussdrv -g
CC= g++
LIBS_PRU=/usr/local/lib
HEADERS_PRU=/usr/local/include


all: pru

clean:
	rm -f example *.o
pru: 
	$(CC) -L$(LIBS_PRU) -I$(HEADERS_PRU)  $(CFLAGS) -Wl,-rpath=$(LIBS_PRU) pru.cpp main.cpp  -o main.o $(LDLIBS)
