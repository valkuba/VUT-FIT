// bitset.c
// Řešení IJC-DU1, příklad a), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Modul s externími definicemi inline funkcí, při spuštění bez optimalizace -O0

#include "bitset.h"

#ifdef USE_INLINE
    
    static inline unsigned long bitset_size(bitset_t jmeno_pole);
    static inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, unsigned long vyraz);
    static inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);

#endif