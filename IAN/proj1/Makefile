#
# Makefile
# Řešení projektu č.1 do předmětu IAN
# Autor: Jakub Valeš, xvales04
#

CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -Wextra

all: ian-proj1

ian-proj1: ian-proj1.o
	$(CC) $(CFLAGS) -g ian-proj1.o -o ian-proj1 -lelf

ian-proj1.o: ian-proj1.c
	$(CC) $(CFLAGS) -c -o ian-proj1.o ian-proj1.c

.PHONY: all

clean:
	rm -rf ian-proj1 ian-proj1.o