#
# Makefile for CSF Assignment 3
#

CFLAGS = -g -Og -Wall -Wextra -pedantic

%.o : %.cpp
	g++ $(CFLAGS) -c $<

all : csim


csim: csim_io.o csim_main.o cache.o
	g++ -o $@ $(CFLAGS) csim_io.o csim_main.o


csim_tests : csim_io.o csim_tests.o tctestpp.o
	g++ -o $@ $ csim_io.o csim_tests.o tctestpp.o

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip
assign3.zip :
	rm -f assign3.zip
	zip -9r $@ Makefile *.h *.c *.cpp

clean :
	rm -f *.o csim assign3.zip
