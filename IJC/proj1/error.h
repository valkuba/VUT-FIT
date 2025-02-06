// error.h
// Řešení IJC-DU1, příklad b), 22.3.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// hlavičkový soubor s prototypy funkcí modulu error.c

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef ERROR_H
#define ERROR_H

    void warning_msg(const char *fmt, ...);
    void error_exit(const char *fmt, ...);

#endif