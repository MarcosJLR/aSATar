.PHONY: all

CC = g++
CFLAGS = -std=c++17 -O2 -Wall

all: sud_sat sat_sud 

sud_sat: sud-sat.cpp
	$(CC) $(CFLAGS) -o $@ $< 

sat_sud: sat-sud.cpp
	$(CC) $(CFLAGS) -o $@ $< 
