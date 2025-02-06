#
# Makefile
# Řešení IJC-DU2, 20.4.2022
# Autor: Jakub Valeš, FIT
#

CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra

all: tail

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

primes.o: tail.c error.c error.h
	$(CC) $(CFLAGS) -c tail.c -o tail.o

primes: tail.o error.o
	$(CC) $(CFLAGS) error.o tail.o -o tail

clean:
	rm -f tail.o error.o

clear:
	rm -f tail xvales04.zip

zip: *.c *.h Makefile
	zip xvales04.zip *.c *.h *.cc Makefile

