#
# Makefile
# Řešení IOS-Projekt2, 2.5.2022
# Autor: Jakub Valeš, FIT
#

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread
PROJ=proj2

all: proj2

$(PROJ): $(PROJ).c $(PROJ).h
	$(CC) $(CFLAGS) $(PROJ).c -o $(PROJ)

clean:
	rm -f $(PROJ) $(PROJ).zip

zip: *.c *.h Makefile
	zip $(PROJ).zip *.c *.h Makefile

