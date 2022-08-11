# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o
	$(CC) $(CFLAGS) -o main main.o
 
# The main.o target can be written more simply
 
main.o: main.cc objects.o utils.o
	$(CC) $(CFLAGS) -c main.cc

objects.o: hitable.h sphere.h

utils.o: vec3.h ray.h

clean:
	rm -rf ./*.o ./*.ppm trace ./*.gch