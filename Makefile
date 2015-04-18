CC	= g++
CFLAGS	= -Wall -Wextra

all: main

main: main.o cache.o

clean:
	rm -f main *.o
