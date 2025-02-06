// error.c
// Řešení IJC-DU1, příklad b), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Definice funkcí error

#include "error.h"

void warning_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}


void error_exit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}
