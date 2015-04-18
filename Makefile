CC	= g++
CFLAGS	= -std=c++11 -Wall -Wextra

all: main

main: main.o cache.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp

cache.o: cache.cpp
	$(CC) $(CFLAGS) -c -o cache.o cache.cpp
clean:
	rm -f main *.o
