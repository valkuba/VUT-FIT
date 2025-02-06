// proj2.h
// Řešení IOS-Projekt2, Synchronizace procesů
// Verze:1, 2.5.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Hlavičkový soubor s knihovnami, definicí struktur a prototypy funkcí

#ifndef PROJ2_H
#define PROJ2_H

    #include <stdio.h>
    #include <stdlib.h>

    #include <sys/types.h>
    #include <sys/mman.h>
    #include <unistd.h>
    #include <stdint.h>
    #include <wait.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <semaphore.h>
    
    #include <string.h>
    #include <time.h>
    #include <stdarg.h>

    typedef struct arg{
        int NO;         // pocet kysliku
        int NH;         // pocet vodiku
        int ti;         // max. čas v ms, ktery pred ceka v queue
        int tb;         // max. čas v ms pro vytvoreni molekuly
        int max_mol;
    } argT;

    argT zpracuj_arg(int argc, char *argv[]);

    void init();

    void cleanup();

    void barrier();

    void my_fprintf(const char *fmt, ...);

    void kyslik_create(int idO, argT input);

    void vodik_create(int idH, argT input);
#endif