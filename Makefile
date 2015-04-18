CC	= g++
CFLAGS	= -std=c++11 -Wall -Wextra

all: main

main: main.o cache.o lru_stack.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c -o main.o main.cpp

cache.o: cache.cpp
	$(CC) $(CFLAGS) -c -o cache.o cache.cpp

lru_stack.o: lru_stack.cpp
	$(CC) $(CFLAGS) -c -o lru_stack.o lru_stack.cpp
clean:
	rm -f main *.o
