/////////////////////////////////////////////////////////////
/*                  1. projekt do IZP:                     */
/*                     Prace s textem                      */
/*                                                         */                                
/*                      Verze: 1                           */
/*                                                         */                                                            
/*                      Jakub Valeš                        */
/*                      xvales04                           */
/*                      31.10.2021                         */
/////////////////////////////////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define input_MAX 101

//Funkce pro kontrolu znaku v hesle - pokud v danem hesle nalezne znak ze zadaneho rozmezi vrati 1
//pro velka pismena nastavim parametry A = 'A', B = 'Z';  pro mala pismena A = 'a', B = 'z';  pro cisla A = '0', B = '9'
int char_check(char password[], char A, char B)
{
    int i = 0;
    while(password[i] != '\0')
    {
        if(password[i] >= A && password[i] <= B)
            return 1;
        else
            i++;
    }
    return 0;
}

//Funkce pro kontrolu specalnich znaku v hesle - vraci 1, pokud nalezne specialni znak
int spec_char_check(char password[])
{
    int i = 0;
    while(password[i] != '\0')
    {
        if(password[i] >= 32 && password[i] <= 126 && !(password[i] >= 'A' && password[i] <= 'Z') && !(password[i] >= 'a' && password[i] <= 'z') && !(password[i] >= '0' && password[i] <= '9'))
            return 1;
        else
            i++;
    }
    return 0;
}

//Funkce pro zjisteni sekvence stejnych znaku 
//Funkce kontroluje znaky na pozicich vedle sebe a pokud jsou stejne pricte do promenne "same" 1, a pokud ne -> nastavi promennou na 0
int same_char_check(char password[], int parametr)
{
    for(int i = 0, same = 0; password[i+1] != '\0'; i++)
    {
        if(password[i] == password[i+1])
            same++;
        else
            same = 0;
        
        if(same == parametr-1)
            return 0;
    }
    return 1;    
}

//Funkce pro zjisteni zda heslo neobsahuje dva stejne podretezce
int same_str_check(char password[], int parametr)
{
    char substring[parametr+1];
    substring[parametr] = '\0';
    char string[input_MAX];
    int j = 0;
    while(password[j] != '\0')
    {
        //Prepisuje heslo do promenne "string"
        for(int a = 0; password[a] != '\0'; a++)
            string[a] = password[a];

        //Do promenne "substring" nacte podretezec z retezce "string", ktery je dlouhy podle zadaneho parametru
        for(int i = 0, x = j; i < parametr; i++, x++)
        {
            substring[i] = string[x];
            string[j] = 8;                                              
            //V retezci "string" nahradi jeden znak (ktery jsem pouzil v promenne "substing") znakem s ordinalni hodnotou 8 - to delam abych pak pri porovnavani neporovnaval podretezec se sebou samym
            //Ordinalni hodnotu 8 ma Backspace, ktery by v zadnem hesle nemel byt
        }
        j++;

        //Hleda jestli se v upravenem  retezci "string" nachazi vybrany podretezec "substing"
        for(int k = 0; string[k] != '\0'; k++)
        {
            //Porovnava znaky v retezci s prvnim znakem v podretezci
            if(string[k] == substring[0])
            {
                for(int l = k, m = 0, same = 0; m < parametr; l++, m++)
                {
                    //Pokud je podminka splnena porovnava dalsi znaky v retezci se znaky v podretezci
                    if(string[l] == substring[m])
                        same++;
                    else
                        same = 0;
                    
                    //Pokud je promenna "same" rovna zadanemu parametru -> v retezci se nachazi podretezec funkce vrati 0
                    if(same == parametr)
                        return 0;
                }
            }
        }
    }
    return 1;
}

//Funkce, do ktere zadam heslo a parametry a funkce nasledne vola predchozi funkce podle zadanych parametru
int password_check(char password[], int parametr1, int parametr2)
{
bool ok = true;
    if(parametr1 >= 1)
        if(!(char_check(password, 'A', 'Z') && char_check(password, 'a', 'z')))
            ok = false;

    if(parametr1 >= 2)
    {
        //Pro parametry [2 1] a [2 2] neni potreba delat vetveni, protoze jsou zahrnuty v parametru [1]
        if(parametr2 >= 3)
            if(!(char_check(password, '0', '9')))
                ok = false;
        if(parametr2 >= 4)
            if(!(spec_char_check(password)))
                ok = false;
    }

    if(parametr1 >= 3)
    {
        if(!(same_char_check(password, parametr2)))
            ok = false;
    }

    if(parametr1 == 4)
    {
        if(!(same_str_check(password, parametr2)))
            ok = false;
    }

    //Vraci 1, kdyz heslo projde kontrolou
    if(ok)
        return 1;
    else
        return 0;
}

//Funkce vraci delku daneho hesla - potrebne pro volitelny parametr --stats
//Prochazi heslo po znaku a za kazdy znak pricte do promenne "len" 1
int stats_lengh(char password[])
{
    int len = 0;    
    for(int i = 0; password[i] != '\0'; i++)
    {
        if(password[i] != '\n')
            len++;
    }
    return len;
}

//Funkce void pro vypocitani prumeru znaku ("AVG") a tisk statistik
void stats_print(int NCHARS, int count, int lengh, int MIN)
{
    double AVG = 0;
    if(count > 0)                                                        //nelze delit 0
        AVG = (double)lengh/count;
    else
        AVG = 0.0;
    printf("Statistika:\nRuznych znaku: %d\nMinimalni delka: %d\nPrumerna delka: %.1f\n", NCHARS, MIN, AVG);
}

int main(int argc, char *argv[])
{    
    //Nacitani a kontrola argumentu z terminalu do promenne "parametr"
    int parametr[2] = {0, 0};
    bool do_stats = false;
    for(int i = 1, j = 0; i < argc; i++)
    {
        //Pro prvni dva ciselne parametry
        if(*argv[i] >= '1' && *argv[i] <= '9') 
            parametr[i-1] = atoi(argv[i]); 
        
        //Pro volitelny parametr '--stats'
        else
        {
            //Porovnava znak po znaku retezec z argumentu s retezcem "stats"
            char stats[8] = "--stats";
            for(j = 0; argv[i][j] != '\0'; j++)
            {
                if(argv[i][j] == stats[j])
                    do_stats = true;
                else
                {
                    //Pro pripad, kdy je volitelny treti parametr zadan, ale chybne
                    fprintf(stderr, "Spatne zadane parametry");
                    do_stats = false;
                    return 1;
                }
            }
        }
    }
    //Chybove hlaseni pro prvni dva argumenty
    if (!(parametr[0] >= 1 && parametr [0] <= 4 && parametr[1] >= 1))
    {
        fprintf(stderr, "Spatne zadane parametry");
        return 1;
    }

    //Nacitani hesel ze souboru "hesla.txt"
    int c;
    char input[input_MAX];
    bool broke = false, ASCII[126];
    int NCHARS = 0, MIN = (input_MAX-1), count = 0, len = 0;
    
    //Vytvorim tabulku pro vsechny ordinalni hodnoty znaku z ASCII tabulky, ktere muzou byt pouzity v hesle <32, 122> a ze zacatku je nastavim na false
    for(int i = 0; i < 126; i++)
        ASCII[i] = false;
    //Nacitam dokud nejsem na konci souboru
    while(!(feof(stdin)))
    {
        int i = 0;
        //Do-while cyklus nacita znaky dokud nenarazi na znak '\n', ktery znaci konec radku 
        do
        {
            //Nacitam pomoci funkce getchar() po znaku
            if((c = getchar()) == EOF)                                  //Protoze funkce feof nejdrive precte EOF a az pote vraci true, musim EOF hlidat i ve funkci pro cteni znaku getchar()
            {
                broke = true;
                break;
            }
            input[i] = (char)c;
            
            //Podminka hlida aby heslo nebylo prilis dlouhe
            if(i >= input_MAX)
            {
                fprintf(stderr, "Heslo je prilis dlouhe");
                return 1;
            }
            input[i+1] = '\0';
            i++;
        }while(c != '\n');

        //Pokud funkce getchar() nasla EOF -> nasel jsem posleni heslo a chci ukoncit cyklus while
        //Ale pokud je soubor s hesly pouze jednoradkovy nechci ukoncit cyklus while, protoze by se heslo nevypsalo
        if(broke && count > 0)
            break;
        
        //STATISTIKA:
        //Promenna "count" slouzi pro pocitani celkoveho poctu retezcu v souboru
        count++;
        //Promenna "MIN" pro nejkratsi retezec, porovnavam delku aktualniho retezce s nejmensim zadanym, pokud je aktualni mensi -> priradim jeho delku do promenne "MIN"
        MIN = (MIN <= stats_lengh(input))? MIN : stats_lengh(input);
        //Do promenne "len" pricitam delky retezcu
        len += stats_lengh(input);
        
        //Pro statistiku ruznych znaku:
        //Pokud v hesle naleznu znak z teto tabulky nastavim jeho hodnotu v tabulce na true
        for(int i = 0; input[i] != '\0'; i++)
        { 
            for(int j = 32; j <= 122; j++)
            {
                if((char)j == input[i])
                {
                    ASCII[j] = true;
                    break;
                }
            }
        }
        
        //Kontrola jestli heslo projde podle zadanych parametru
        if(password_check(input, parametr[0], parametr[1]))
            printf("%s", input);
        else
            continue;
    }

    if(do_stats)
    {
    //Promenna "NCHARS" pocita pocet polozek v tabulce s hodnotou true
    for(int i = 0; i <= 126; i++)
    {
        if(ASCII[i])
            NCHARS++;
    }
    //Volani funkce pro tisk statistik
    stats_print(NCHARS, count, len, MIN);
    }
    return 0;
}