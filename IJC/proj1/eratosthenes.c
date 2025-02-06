// eratosthenes.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Definice funkce pro Eratosthenovo síto

#include "eratosthenes.h"
#include <math.h>

void Eratosthenes(bitset_t pole)
{
    for(unsigned long i = 2; i < sqrt(bitset_size(pole)); i++)
    {
        if(bitset_getbit(pole, i) == 0)
            for(unsigned long j = i + i; j < bitset_size(pole); j += i)
            {
                bitset_setbit(pole, j, 1UL);
            }
    }
}

