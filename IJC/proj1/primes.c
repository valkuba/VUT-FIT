// primes.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Výpis posledních 10 prvočísel

#include "eratosthenes.h"
#include <time.h>

#define SIZE 300000000UL
int main(void)
{
    clock_t start = clock();
    bitset_create(pole, SIZE);
    Eratosthenes(pole);

    int pocet = 0;
    unsigned long prvocisla[10] = {0, };
    
    // Prochazí pole od konce a poslednich 10 prvočísel ukládá do pole prvočísel
    for(unsigned long i = bitset_size(pole) - 1; i > 2; i--)
    {
        if(bitset_getbit(pole, i) == 0)
        {
            prvocisla[pocet] = i;
            pocet++;
            
            if(pocet == 10)
                break;
        }
    }

    //Výpis prvočísel
    for(int j = pocet - 1; j >= 0; j--)
        printf("%ld\n", prvocisla[j]);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);   
}
