.PHONY: all, clean

CC = g++
CFLAGS = -std=c++17 -O2 -Wall
OBJS = Solver.o

all: solver clean

solver: main.cpp $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(OBJS)

Solver.o: Solver.cpp Solver.hpp SolverTypes.hpp
	$(CC) $(CFLAGS) -c -o $@ $<	

clean:
	rm *.o