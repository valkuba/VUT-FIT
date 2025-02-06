// proj2.c
// Řešení IOS-Projekt2, Synchronizace procesů
// Verze:1, 2.5.2022
// Autor: Jakub Valeš, FIT
// Přeloženo: gcc 9.3.0

#include "proj2.h"
#define PAGESIZE 4096 

FILE *soubor;

// sdilená paměť:
int *kyslik_poc = NULL;
int *vodik_poc = NULL;
int *molekule_poc = NULL;
int *cislo_rad = NULL;
int *v_i_q = NULL;          // počet všech vodíků v queue
int *k_i_q = NULL;          // počet všech kyslíků v queue
int *count = NULL;          // počet procesů, na které se čeká pro barrieru

sem_t *sem_write, *sem_mutex, *sem_vodik_Q, *sem_kyslik_Q, *turnstile, *turnstile2, *mutex2, *sem_end, *sem_created;
// konec

// Zpracování argumentů z parametru *argv[] a nahrání do struktury argT
argT zpracuj_arg(int argc, char *argv[])
{
    argT argumenty;
    if(argc != 5)
    {
        fprintf(stderr, "ERROR: Špatný počet argumentů, formát spuštění s argumenty: ./proj2 NO NH TI TB\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        char *pEND;
        argumenty.NO = strtol(argv[1], &pEND, 10);
        if((strlen(pEND) != 0) || (argumenty.NO <= 0))
        {
            fprintf(stderr, "ERROR: Argumenty mohou být pouze přirozená čísla\n");
            exit(EXIT_FAILURE);
        }
        
        argumenty.NH = strtol(argv[2], &pEND, 10);
        if((strlen(pEND) != 0) || (argumenty.NH <= 0))
        {
            fprintf(stderr,"ERROR: Argumenty mohou být pouze přirozená čísla\n");
            exit(EXIT_FAILURE);
        }
        
        argumenty.ti = strtol(argv[3], &pEND, 10);
        if((strlen(pEND) != 0) || strcmp(argv[3], "") == 0 || argumenty.ti < 0 || argumenty.ti > 1000)
        {
            fprintf(stderr,"ERROR: Špatný formát argumentu TI, správně: TI je číslo < 0 && > 1000\n");
            exit(EXIT_FAILURE);
        }
        
        argumenty.tb = strtol(argv[4], &pEND, 10);
        if((strlen(pEND) != 0) || strcmp(argv[4], "") == 0 || argumenty.tb < 0 || argumenty.tb > 1000)
        {
            fprintf(stderr,"ERROR: Špatný formát argumentu TB, správně: TB je číslo < 0 && > 1000\n");
            exit(EXIT_FAILURE);
        }
    }
    // Vypočítám kolik maximálně můžu ze zadaných prvků vytvořit molekul
    argumenty.max_mol = (argumenty.NO) < (argumenty.NH/2) ? (argumenty.NO) : (argumenty.NH/2);

    return argumenty;
}

// Inicializace sdílené paměti
void init()
{
    soubor = fopen("proj2.out", "w");
    if(soubor == NULL)
    {
        fprintf(stderr,"Error open :(\n");
        exit(EXIT_FAILURE);
    }

    // Inicializace sdílených proměnných
    molekule_poc = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *molekule_poc += 1;
    kyslik_poc = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    vodik_poc = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    cislo_rad = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    v_i_q = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    k_i_q = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    count = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


    // pokud již existuji nějaké semafory s těmito názvy, tak je smažu
    sem_unlink("IOS_xvales04_sem_mutex");
    sem_unlink("IOS_xvales04_sem_write");
    sem_unlink("IOS_xvales04_sem_vodik_Q");
    sem_unlink("IOS_xvales04_sem_kyslik_Q");
    sem_unlink("IOS_xvales04_sem_turnstile");
    sem_unlink("IOS_xvales04_sem_turnstile2");
    sem_unlink("IOS_xvales04_sem_mutex2");
    sem_unlink("IOS_xvales04_sem_end");

    // Vytvoření semaforů a jejich error handling
    // Semafor pro zápis do souboru
    // -> aby nedošlo ke kolizi, kdy dva procesy zapisují současně
    sem_write = sem_open("IOS_xvales04_sem_write", O_CREAT | O_EXCL, 06660, 1);
        if(sem_write == SEM_FAILED)
        {
            fprintf(stderr,"sem wr error\n");
            exit(EXIT_FAILURE);
        }

    sem_mutex = sem_open("IOS_xvales04_sem_mutex", O_CREAT | O_EXCL, 06660, 1);
        if(sem_mutex == SEM_FAILED)
        {
            fprintf(stderr,"sem mt error\n");
            exit(EXIT_FAILURE);
        }

    // Semafor pro vodíky/kyslíky aby po vytvoření čekali, než jich bude dostatečný počet pro molekulu
    sem_vodik_Q = sem_open("IOS_xvales04_sem_vodik_Q", O_CREAT | O_EXCL, 06660, 0);
        if(sem_vodik_Q == SEM_FAILED)
        {
            fprintf(stderr,"sem H error\n");
            exit(EXIT_FAILURE);
        }

    sem_kyslik_Q = sem_open("IOS_xvales04_sem_kyslik_Q", O_CREAT | O_EXCL, 06660, 0);
        if(sem_kyslik_Q == SEM_FAILED)
        {
            fprintf(stderr,"sem O error\n");
            exit(EXIT_FAILURE);
        }

    // 3 semafory pro barieru:
    turnstile = sem_open("IOS_xvales04_sem_turnstile", O_CREAT | O_EXCL, 06660, 0);
        if(turnstile == SEM_FAILED)
        {
            fprintf(stderr,"sem t1 error\n");
            exit(EXIT_FAILURE);
        }


    turnstile2 = sem_open("IOS_xvales04_sem_turnstile2", O_CREAT | O_EXCL, 06660, 1);
        if(turnstile2 == SEM_FAILED)
        {
            fprintf(stderr,"sem t2 error\n");
            exit(EXIT_FAILURE);
        }
        
    mutex2 = sem_open("IOS_xvales04_sem_mutex2", O_CREAT | O_EXCL, 06660, 1);
        if(mutex2 == SEM_FAILED)
        {
            fprintf(stderr,"sem mt2 error\n");
            exit(EXIT_FAILURE);
        }

    // Semafor čeká až se vytvoří poslední molekula, aby mohla být vypsána zpráva "not enough"
    // pozn. Podle fóra nejspíše není potřeba čekat, ale vypadá to úhledněji
    sem_end = sem_open("IOS_xvales04_sem_end", O_CREAT | O_EXCL, 06660, 0);
        if(sem_end == SEM_FAILED)
        {
            fprintf(stderr,"sem end error\n");
            exit(EXIT_FAILURE);
        }
}

// Pro uvolnění veškeré sdílené paměti a uzavření souboru
void cleanup()
{

    munmap(vodik_poc, sizeof(*vodik_poc));
    munmap(kyslik_poc, sizeof(*kyslik_poc));
    munmap(molekule_poc, sizeof(*molekule_poc));
    munmap(cislo_rad, sizeof(*cislo_rad));
    munmap(v_i_q, sizeof(*v_i_q));
    munmap(k_i_q, sizeof(*k_i_q));
    munmap(count, sizeof(*count));

    sem_close(sem_write);
    sem_unlink("IOS_xvales04_sem_write"); 
    
    sem_close(sem_mutex);
    sem_unlink("IOS_xvales04_sem_mutex");

    sem_close(sem_vodik_Q);
    sem_unlink("IOS_xvales04_sem_vodik_Q");

    sem_close(sem_kyslik_Q);
    sem_unlink("IOS_xvales04_sem_kyslik_Q");

    sem_close(turnstile);
    sem_unlink("IOS_xvales04_sem_turnstile");

    sem_close(turnstile2);
    sem_unlink("IOS_xvales04_sem_turnstile2");

    sem_close(mutex2);
    sem_unlink("IOS_xvales04_sem_mutex2");

    sem_close(sem_end);
    sem_unlink("IOS_xvales04_sem_end");
    
    if(fclose(soubor) == EOF)
    {
        fprintf(stderr, "Soubor s výstupem se nepodařilo uzavřít");
        exit(EXIT_FAILURE);
    }
}


void my_fprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    sem_wait(sem_write);
        *cislo_rad += 1;
        fprintf(soubor, "%d: ", *cislo_rad);
        vfprintf(soubor, fmt, ap);
        fflush(soubor);
    sem_post(sem_write);
    va_end(ap);
}

// Reusable barrier
// Pro synchronizaci více procesů:
// Čeká až všecny 3 procesy, které v tomto případě vytváří molekulu budou ukončeny
// a může začít vytváření další molekuly
void barrier()
{
    sem_wait(mutex2);
        *count += 1;
        if(*count == 3)
        {
            sem_wait(turnstile2);
            sem_post(turnstile);
        }
    sem_post(mutex2);

    sem_wait(turnstile);
    sem_post(turnstile);


    sem_wait(mutex2);
        *count -= 1;
        if(*count == 0)
        {
            sem_wait(turnstile);
            sem_post(turnstile2);
        }
    sem_post(mutex2);

    sem_wait(turnstile2);
    sem_post(turnstile2);
}

// Vytváření kyslíku a spojovaní molekul
void kyslik_create(int idO, argT input)
{
    srand(time(NULL) * getpid());
    if(*molekule_poc == input.max_mol+1)
        sem_post(sem_end);

    // Začátek vytváření kyslíku
    my_fprintf("O %d: started\n", idO);
    usleep(rand() % (input.ti+1)*1000);

    // Zařazení kyslíku do queue
    my_fprintf("O %d: going to queue\n", idO);

    sem_wait(sem_mutex);
    *kyslik_poc += 1;
    *k_i_q += 1;
    // Kontrola jesli má daný kyslík šanci vytvořit molekulu (zda-li je/bude dostatek vodíků)
    if(*k_i_q > input.max_mol)
    {
        sem_post(sem_mutex);
        sem_wait(sem_end);
        sem_wait(sem_mutex);
            *kyslik_poc -= 1;
            *k_i_q -= 1;
            my_fprintf("O %d: not enough H\n", idO);
        sem_post(sem_end);
        sem_post(sem_mutex);
        exit(0);
    }
    else
    {
        // Pokud jsou s kyslíkem v queue další dva vodíky můžu vytvořit molekulu
        if(*vodik_poc >= 2)
        {
            sem_post(sem_vodik_Q);
            sem_post(sem_vodik_Q);
            *vodik_poc -= 2;

            sem_post(sem_kyslik_Q);
            *kyslik_poc -= 1;
        }
        else
            // Pokud nejsou pustí do segmentu možnosti vytváření další proces a počká na semafor sem_kyslik_Q
            sem_post(sem_mutex);
    }

    sem_wait(sem_kyslik_Q);

    // Vytváření molekuly
    my_fprintf("O %d: creating molecule %d\n", idO, *molekule_poc);
    usleep(rand() % (input.tb+1)*1000);

    // Bariera synchronizuje všechny 3 výpisy CREATING a nechá vypsat zprávu CREATED
    barrier();
    my_fprintf("O %d: molecule %d created\n", idO, *molekule_poc);
    
    // Bariera synchronizuje konec všech procesů CREATED a je možnost inkrementovat počítadlo vytvořených molekul
    barrier();
    *molekule_poc +=1;
    
    // Pokud se již vytvořil předpokládaný počet molekul odemknu semafor sem_end, aby se mohly vypsat zprávy NOT ENOUGH
    if(*molekule_poc == input.max_mol+1)
        sem_post(sem_end);

    sem_post(sem_mutex);
    
    if(fclose(soubor) == EOF)
    {
        fprintf(stderr, "Soubor s výstupem se nepodařilo uzavřít");
        exit(EXIT_FAILURE);
    }
    
}

// Vytváření vodíku a spojování molekul
void vodik_create(int idH, argT input)
{
    srand(time(NULL) * getpid());
    
    // Začátek vytváření vodíku
    my_fprintf("H %d: started\n", idH);

    usleep(rand() % (input.ti+1)*1000);

    // Vodík jde do queue    
    my_fprintf("H %d: going to queue\n", idH);

    // Segment vytváření molekuly / čekání na prvky a vytváření
    sem_wait(sem_mutex);
    *vodik_poc += 1;
    *v_i_q += 1;
    if(*v_i_q > input.max_mol*2)
    {
        sem_post(sem_mutex);
        sem_wait(sem_end);
        sem_wait(sem_mutex);
            *vodik_poc -= 1;
            *v_i_q -= 1;
            my_fprintf("H %d: not enough O or H\n", idH);
        sem_post(sem_end);
        sem_post(sem_mutex);
        exit(0);
    }
        else
    {        
        if(*vodik_poc >= 2 && *kyslik_poc >= 1)
        {
            sem_post(sem_vodik_Q);
            sem_post(sem_vodik_Q);
            *vodik_poc -= 2;

            sem_post(sem_kyslik_Q);
            *kyslik_poc -= 1;
        }
        else
            sem_post(sem_mutex);
    }

    sem_wait(sem_vodik_Q);

    my_fprintf("H %d: creating molecule %d\n", idH, *molekule_poc);
    
    barrier();
    my_fprintf("H %d: molecule %d created\n", idH, *molekule_poc);

    barrier();
    
    if(fclose(soubor) == EOF)
    {
        fprintf(stderr, "Soubor s výstupem se nepodařilo uzavřít");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    argT input = zpracuj_arg(argc, argv);
    init();

    // Hlavní proces vytvoří NH procesů pro vodík a NO procesů pro kyslík
    for(int idH = 1; idH <= input.NH; idH++)
    {
        pid_t PID_HYD = fork();
        if(PID_HYD == 0)    // potomek
        {
            vodik_create(idH, input);
            exit(EXIT_SUCCESS);
        }
        else if(PID_HYD == -1)  // chyba
        {
            fprintf(stderr,"Funkce fork() selhala\n");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    for(int idO = 1; idO <= input.NO; idO++)
    {
        pid_t PID_OXY = fork();
        if(PID_OXY == 0)
        {
            kyslik_create(idO, input);
            exit(EXIT_SUCCESS);
        }
        else if(PID_OXY == -1)
        {
            fprintf(stderr,"Funkce fork() selhala\n");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    // hlavní proces čeká, až se všechny procesy potomků ukončí
    while(wait(NULL) > 0);

    cleanup();
    
    return EXIT_SUCCESS;
}