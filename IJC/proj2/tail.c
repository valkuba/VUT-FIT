// bitset.c
// Řešení IJC-DU2, příklad a), 20.4.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0
// Program tail, který vypisuje 10 posledních řádků ze souboru / stdin, volitelný přepínač -n  a číslo určuje, kolik řádků bude vypsáno (default=10)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.c"

#define BUF_MAX 4095
#define FILE_MAX 256

//struktura pro zpracování argumentů
typedef struct argument
{
    char nazev_souboru[FILE_MAX];
    bool soubor;
    int n;
    FILE *fp;
} argT;

// funkce pro zpracování argumentů, vrací strkuturu, ve které jsou argumenty uloženy
argT zpracuj_arg(int argc, char *argv[])
{
    char *err = NULL;
    argT argumenty ={.n=10, .soubor = false};
    if(argc > 4)
        error_exit("Příliš mnoho argumentů\n");
    else
    {
        for(int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], "-n") == 0)
            {
                if((i+1) >= argc)
                    error_exit("Chybí parametr size\n");
                else
                    argumenty.n = strtol(argv[++i], &err, 10);
                if(err == NULL || argumenty.n < 0)
                    error_exit("Špatně zadaný parametr size\n");
            }
            else
            {
                if(strlen(argv[i]) > FILE_MAX)
                    error_exit("Nazev souboru je příliš dlouhy, maximalně %d znaků\n", FILE_MAX);
                else
                {
                    strcpy(argumenty.nazev_souboru, argv[i]);
                    argumenty.soubor = true;
                }
            }
            
        }
    }
    return argumenty;
}

// funkce naalokuje paměť pro ukazatel na pole řetězců
// při úspěšné alokaci vrací tento ukazatel
char ** ctor_pole_string()
{
    char **pole_string = (char **) malloc(1 * sizeof(char*));
    if(pole_string == NULL)
    {
        error_exit("Alokace paměti selhala\n");
    }
    return pole_string;
}

// funkce pro uvolnění paměti
void destruktor(char **pole_string, int pocet_radku)
{
    for(int i = 0; i < pocet_radku; i++)
    {
        free(pole_string[i]);
    }
    free(pole_string);
}

// funkce alokuje potřebnou paměť pro řádek načtený ze souboru
// při úspěchu vrací ukazatel na řetězec
// při chybě alokace se zbytek naalokované paměti uvolní
char * inicial_pole_string(char **pole_string, int pocet_radku, size_t velikost, argT argumenty)
{
    pole_string[pocet_radku] = (char *) malloc(velikost * sizeof(char));
    if(pole_string[pocet_radku] == NULL)
    {
        destruktor(pole_string, pocet_radku);
        if(argumenty.soubor)
            fclose(argumenty.fp);
        error_exit("Alokace selhala\n");
    }
    return pole_string[pocet_radku];
}

// funkce zvětší naalokovanou paměť pro ukazatel na pole řeťezců
// při úspěšné alokaci vrací ukazatel na pole řeťezců
char ** realloc_pole_string(char **pole_string, int pocet_radku, argT argumenty)
{
    char **ptr;
    ptr = (char **) realloc(pole_string, (pocet_radku + 1) * sizeof(char*));
    if(ptr == NULL)
    {
        if(argumenty.soubor)
            fclose(argumenty.fp);
        error_exit("Alokace selhala\n");
    }
    pole_string = ptr;
    return pole_string;
}

// funkce tiskne posledních n řádků načtených ze souboru (pokud jich tolik má k dispozici)
void tisk_tail(char **pole_string, int pocet_radku, int pocet_tisk)
{
    if(pocet_tisk > pocet_radku)
    {
        destruktor(pole_string, pocet_radku);
        error_exit("Nelze tisknout více řádku, než je v souboru\n");
    }
    else
    {
        for(int i = (pocet_radku-pocet_tisk); i < pocet_radku; i++)
            printf("%s", pole_string[i]);
    }
}

// načtení řádků ze souboru a uložení jich do ukazatele na pole řetězců
// vrací ukazatel na pole řetězců
char ** nacti_radky(argT argumenty, char **pole_string, int *pocet_radku)
{
    char buffer[BUF_MAX];
    bool overflow = false;
    bool zprava = false;
    while(fgets(buffer, sizeof(buffer), argumenty.fp) != NULL)
    {
        // pokud je řádek delší než buffer -> načtený řádek nekončí /n
        if(buffer[strlen(buffer)-1] != '\n')
        {
            buffer[strlen(buffer)-1] = '\n';
            if(!zprava)
            {
                warning_msg("Radek je delsi nez %d, pokracuji s limitem %d\n", BUF_MAX, BUF_MAX);
                zprava = true;
            }
            overflow = true;
        }
        
        pole_string[*pocet_radku] = inicial_pole_string(pole_string, *pocet_radku, (strlen(buffer) + 1), argumenty);
        strcpy(pole_string[*pocet_radku], buffer);
        *pocet_radku+=1;
        pole_string = realloc_pole_string(pole_string, *pocet_radku, argumenty);
        
        // pokud je řádek delší, načte se pouze tolik znaků, kolik se vejde do buffer
        // zbytek se načte "na prázdno" a nikam se neuloží
        if(overflow)
        {
            do{
                fgets(buffer, sizeof(buffer), argumenty.fp);
            }while(buffer[strlen(buffer)-1] != '\n');
            overflow = false;
        }

    }
    return pole_string;
}

int main(int argc, char *argv[])
{
    argT argumenty = zpracuj_arg(argc, argv);
    char **pole_string = ctor_pole_string();

    int pocet_radku = 0;
    // pokud je zadán název souboru jako argument, otevřu soubor a čtu z něj
    if(argumenty.soubor)
    {
        argumenty.fp = fopen(argumenty.nazev_souboru, "r");
            if(argumenty.fp == NULL)
            {
                free(pole_string);
                error_exit("Nepodařilo se otevřít soubor\n");
            }
        
        pole_string = nacti_radky(argumenty, pole_string, &pocet_radku);
        fclose(argumenty.fp);
    }
    // pokud není zadán soubor, čtu ze stdin
    else
    {
        argumenty.fp = stdin;
        pole_string = nacti_radky(argumenty, pole_string, &pocet_radku);
    }

    // tisk a uvolnění paměti
    tisk_tail(pole_string, pocet_radku, argumenty.n);
    destruktor(pole_string, pocet_radku);
}