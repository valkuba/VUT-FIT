// bitset.h
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// hlavičkový soubor s definicemi maker a inline funkcemi

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "error.h"

#ifndef BITSET_H
#define BITSET_H

    typedef unsigned long bitset_t[];

    typedef unsigned long bitset_index_t;

        // Automaticka alokace pole na zasobniku a inicializace
        #define bitset_create(jmeno_pole, velikost)                                                         \
                static_assert((velikost) > 0 || (velikost < ULLONG_MAX),                                    \
                "Nelze definovat pole o dane velikosti");                                                   \
                unsigned long jmeno_pole[(((velikost) % (sizeof(unsigned long) * CHAR_BIT))                 \
                ? (((velikost) / (sizeof(unsigned long) * CHAR_BIT)) + 2)                                   \
                : (((velikost) / (sizeof(unsigned long) * CHAR_BIT)) + 1))] = {velikost, 0, }

                // jeden prvek unsigned long pole má sizeof(unsigned long)*8 bitů, přičítám 1 kvůli prvku pro velikost
                // pokud velikost není dělitelná 64 bezezbytku musím pole ještě o jedna zvýšit "pro zbytek po dělení"
                

        // Dynamicka alokace pole na HEAPu
        #define bitset_alloc(jmeno_pole, velikost)                                                          \
                assert((velikost) < ULONG_MAX);                                                             \
                unsigned long *jmeno_pole = malloc(((velikost) % (sizeof(unsigned long) * CHAR_BIT))        \
                ? (((velikost) / (sizeof(unsigned long) * CHAR_BIT)) + 2) * sizeof(unsigned long)           \
                : (((velikost) / (sizeof(unsigned long) * CHAR_BIT)) + 1) * sizeof(unsigned long));         \
                if(jmeno_pole == NULL)                                                                      \
                    error_exit("Chyba alokace paměti");                                                     \
                jmeno_pole[0] = velikost;                                                                   \
                do                                                                                          \
                {                                                                                           \
                for(unsigned long i = 1; i < ((velikost) / (sizeof(unsigned long) * CHAR_BIT)) + 1; i++)    \
                    jmeno_pole[i] = 0;                                                                      \
                } while(0)
                
                

        #define bitset_free(jmeno_pole)                                                                     \
                free(jmeno_pole)


    #ifndef USE_INLINE

        #define bitset_size(jmeno_pole)                                                                    \
                jmeno_pole[0]


        /* Nastaví hodnotu bitu na indexu na 0/1
           bitově posunu 1 na pozici shoudnou s indexem
           a vymaskuji s hodnotou pole (pomocí bitového OR nebo "implikce - negace + OR")
        */
        #define bitset_setbit(jmeno_pole, index, vyraz)                                                    \
                do                                                                                         \
                {                                                                                          \
                    if(index >= (bitset_size(jmeno_pole)))                                                 \
                        error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu",                          \
                        (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));                     \
                    unsigned long posun_set = (unsigned long) (vyraz)                                      \
                    << ((index) % (sizeof(unsigned long) * CHAR_BIT));                                     \
                    if (vyraz)                                                                             \
                        (jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1UL] |= posun_set);   \
                    else                                                                                   \
                    {                                                                                      \
                        jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1UL]                   \
                        = ~(jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1UL]) | posun_set; \
                    }                                                                                      \
                } while(0)

                // Funguje i pouze s použitím implikace (druhý výraz), ale výrazně zpomaluje výpočet modulu primes.c
                // -> z tohoto důvodu jsem pro změnu bitu na 1 využil pouze bitový OR


        /* Zjisti hodnotu bitu na indexu
           Vytvořím bitovou masku tak, že posunu 1UL na bit s indexem
           a pak pomocí bitového AND vymaskuji */
        #define bitset_getbit(jmeno_pole, index)                                                       \
                (((jmeno_pole[(index) / (sizeof(unsigned long) * CHAR_BIT) + 1])                       \
                & ((1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT))))) != 0)


    // INLINE FUNKCE
    #else
        static inline unsigned long bitset_size(bitset_t jmeno_pole)
        {
            return jmeno_pole[0];
        }

        static inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, unsigned long vyraz)
        {
            if(index >= (bitset_size(jmeno_pole)))
                error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));

            unsigned long posun_set = vyraz << (index % (sizeof(unsigned long) * CHAR_BIT));
            if(vyraz)
                jmeno_pole[(index / (sizeof(unsigned long) * CHAR_BIT)) + 1] |= posun_set;
            else
                jmeno_pole[(index / (sizeof(unsigned long) * CHAR_BIT)) + 1] = ~jmeno_pole[(index / (sizeof(unsigned long) * CHAR_BIT)) + 1] | posun_set;
        }

        static inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
        {
            if(index >= (bitset_size(jmeno_pole)))
                error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));
            
            unsigned long posun_get = jmeno_pole[(index / (sizeof(unsigned long) * CHAR_BIT)) + 1];
            posun_get = posun_get >> (index % (sizeof(unsigned long) * CHAR_BIT));
            if((posun_get) & (1UL))
                return 1;
            else
                return 0;
        }

    #endif
#endif