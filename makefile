CC=g++
DEBUG=-g
CFLAGS=-Wall
COMPILER=-std=c++11

default: HCPPP

HCPPP: main.cpp clase.cpp funciones.cpp
	$(CC) $^ $(DEBUG) $(CFLAGS) $(COMPILER) -o $@

clean:
	rm HCPPP

run:
	./HCPPP