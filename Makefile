CC=g++
SRC_DIR=src
FLAGS=-m64 -O3 -Wall -std=c++11
PROG_NAME=queue

all: main

main: Handler.o Watcher.o Logger.o
	$(CC) $(FLAGS) $(SRC_DIR)/main.cpp Handler.o Watcher.o Logger.o LogPool.o -o $(PROG_NAME) -lpthread

Handler.o:
	$(CC) $(FLAGS) -c $(SRC_DIR)/Handler/Handler.cpp

Watcher.o:
	$(CC) $(FLAGS) -c $(SRC_DIR)/Watcher/Watcher.cpp

Logger.o:
	$(CC) $(FLAGS) -c $(SRC_DIR)/ResultLog/*.cpp -lpthread

clean:
	rm -rf *.o $(PROG_NAME)
