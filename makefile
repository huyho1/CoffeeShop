# This makefile is used as follows to regenerate files for the program:
#   make simulation.o   	--  Regenerates simulation.o by compiling
#   make shopQueue.o        --  Regenerates shopQueue.o by compiling
#   make run_simulation     --  Regenerates the executable run_simulation file by compiling
#   make                   	--  Same as "make run_simulation"

run_simulation: simulation.o shopQueue.o
	g++ -g -Wall simulation.o shopQueue.o -lm -o run_simulation

shopQueue.o: shopQueue.h customer.h shopQueue.cpp
	g++ -g -Wall -c shopQueue.cpp

simulation.o: shopQueue.h customer.h shopQueue.cpp
	g++ -g -Wall -c simulation.cpp

clean:
	rm -f shopQueue.o simulation.o run_simulation

all: run_simulation