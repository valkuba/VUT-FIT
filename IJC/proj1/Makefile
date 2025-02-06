#
# Makefile
# Řešení IJC-DU1, 22.3.2022
# Autor: Jakub Valeš, FIT
#

CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall -Wextra

all: primes

eratosthenes.o: eratosthenes.c eratosthenes.h bitset.h error.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c -o error.o

bitset.o: bitset.c bitset.h
	$(CC) $(CFLAGS) -c bitset.c -o bitset.o

primes.o: primes.c eratosthenes.h error.h bitset.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o

primes: primes.o eratosthenes.o error.o bitset.o
	$(CC) $(CFLAGS) eratosthenes.o error.o primes.o -lm -o primes
	$(CC) $(CFLAGS) -DUSE_INLINE eratosthenes.o error.o primes.o bitset.o -lm -o primes-i

run: primes primes-i
	./primes
	./primes-i

clean:
	rm -f primes.o error.o eratosthenes.o bitset.o

clear:
	rm -f primes primes-i xvales04.zip

zip: *.c *.h Makefile
	zip xvales04.zip *.c *.h Makefile

