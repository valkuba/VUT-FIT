// ian-proj1.c
// Řešení projektu č.1 předmětu IAN
// Autor: Jakub Valeš, xvales04
// Datum: 10.3.2024
// Přeloženo: gcc 9.4.0
// Spuštění: ./ian-proj1 <FILE>
// Program ian-proj1 výpiše názvy a hodnoty globálních proměnných ELF souboru přidaného v argumentu <FILE>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <libelf.h>
#include <gelf.h>

// Structure to save info about global variables from .data section
typedef struct global_data{
    int *var_values;                 // values of global variables
    int count;
    long unsigned int start_addr;       // address of fist global variable
} global_dataT;

// Function to convert hexadecimal number in little endian to decimal
// Takes array of unsigned chars representing hex number and returns decimal
int conv_hex_to_int(unsigned char *hex_array){
    unsigned char tmp;
    char str_tmp[20]= {'\0',};
    char str[20] = {'\0',};
    size_t len;
    unsigned long int result;

    len = strlen((const char *)hex_array);
    // Converts hex little endian to hex in big endian 
    for(size_t i = 0; i < len / 2; i++){
        tmp = hex_array[i];
        hex_array[i] = hex_array[len - 1 - i];
        hex_array[len - 1 - i] = tmp;
    }
    // Converts array of hex numbers into one strings
    for(size_t i = 0; i < len; i++){
        sprintf(str_tmp, "%x", hex_array[i]);
        i == 0 ? strcpy(str, str_tmp): strcat(str, str_tmp);
    }
    result = strtol(str, NULL, 16);

    return result;
}

// Function which is called if there should be some global variables but none is printed
// Because if the file is exe the first row of .data section arent global variables info and it should be skipped
// So this function sets the address of first global variable to the second row of .data section (address+16)
// and skips the values which are on the first row of .data
void set_globals(global_dataT *glob_vars)
{

    glob_vars->start_addr += 16;
    for(int i = 0; i < glob_vars->count - 4; i++){
        glob_vars->var_values[i] = glob_vars->var_values[i+4];
    }
    glob_vars->count -= 4;      // because the first 4 values in .data arent global variables
}

// Function which allocates memory for the array of integers in glob_vars struct
// Takes the structure as an argument by reference and argument flag which is decising if malloc or realloc will be used
void alloc_mem(global_dataT *glob_vars, int flag){
    if (flag){
        glob_vars->var_values = malloc(sizeof(int));
        if (glob_vars->var_values == NULL){
            fprintf(stderr, "ERROR: Malloc fail");
            exit(99);
        }
    } else {
        int *ptr = realloc(glob_vars->var_values, (glob_vars->count + 1) * sizeof(int));
        if (ptr == NULL){
            free(glob_vars->var_values);
            fprintf(stderr, "ERROR: Malloc fail");
            exit(99);
        }
        glob_vars->var_values = ptr;
    }
}

// Function to examine the .data section of ELF file and return the information about global variables
// Returns structure glob_vars which includes the starting address of first global and values of globals with their amount
global_dataT parse_data_sec(Elf *elf){
    Elf_Scn *scn = NULL;
    Elf_Data *data;
    GElf_Shdr shdr;
    size_t i, shstrndx;
    char *name;
    unsigned char hex_arr[5] = {'\0',};
    global_dataT glob_vars = {.count = 0};
    alloc_mem(&glob_vars, 1);

    // Iterates through sections of ELF and try to find the .data section
    while ((scn = elf_nextscn(elf, scn)) != NULL){
        if (gelf_getshdr(scn, &shdr) != NULL){
            elf_getshdrstrndx(elf, &shstrndx);
            name = elf_strptr(elf, shstrndx, shdr.sh_name);
            // .data section was found
            if (strcmp(name, ".data") == 0){
                data = elf_getdata(scn, NULL);
                // Get the data from .data section into char buffer so its easier to work
                unsigned char *buf = (unsigned char *)data->d_buf;
                // Get the address from .data section
                glob_vars.start_addr = shdr.sh_addr;
                // Iterate through .data section and save values of global variables
                for (i=0; i < shdr.sh_size; ++i){
                    hex_arr[i % 4] = buf[i];
                    // One whole value of global var was loaded -> call the function to convert it
                    if((i + 1) % 4 == 0){
                        alloc_mem(&glob_vars, 0);
                        glob_vars.var_values[glob_vars.count++] = conv_hex_to_int(hex_arr);
                    }
                }
                break;
            }
        }
    }
    return glob_vars;
}

// Function to analyze the .symtab section of ELF file
// Finds the global variable on given address and prints its name with value
// takes structure glob_vars as an arguments where is the address stored with values of global variables
void find_in_symtab(Elf *elf, global_dataT glob_vars){
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;

    // Iterates through sections of ELF and try to find the .symtab section
    while ((scn = elf_nextscn(elf, scn)) != NULL){
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB)
            break;
    }

    data = elf_getdata(scn, NULL);
    int j = 0;                      // Helps to iterated through the addresses
    int printed = 0;                // How many global vars were printed
    int count = shdr.sh_size / shdr.sh_entsize;
    
    // Iterates through the .symtab section by rows and get information about each row
    for (int i = 0; i < count; i++){
        GElf_Sym sym;
        gelf_getsym(data, i, &sym);
        // If the global variable was found on given address go to another adress
        if (sym.st_value > glob_vars.start_addr && printed > j)
            j++;
        // Checkes if address of global var is on given row
        if (sym.st_value == glob_vars.start_addr + (j*4) && sym.st_size == 4){      // the size of int global variable is 4
            // And prints the name and value of global variable
            printf("%s\t", elf_strptr(elf, shdr.sh_link, sym.st_name));
            printf("%d\n", glob_vars.var_values[(sym.st_value - glob_vars.start_addr)/4]);
            ++printed;
        }
        if (i == count - 1){
            // Because in execetuable file the global variables arent in the same order as in the .data sectioni, its needed to restart the interation
            if (printed < glob_vars.count-1)
                i=0;
            // If none of the global variables were printed but there are some there could be a problem with starting address
            if (printed == 0 && glob_vars.count != 0)
                set_globals(&glob_vars);
        }
        if (printed == glob_vars.count)
            break;
    }
}


int main(int argc, char *argv[]){
    int file;
    Elf *elf;
    global_dataT glob_vars;

    if (argc != 2){
        fprintf(stderr, "ERROR: Need 1 argument as an File name");
        exit(99);
    }
    // Next functions are to open the ELF file for reading
    if (elf_version(EV_CURRENT) == EV_NONE){
        fprintf(stderr, "ERROR: libelf.h inicialization failed, %s", elf_errmsg(-1));
        exit(99);
    }
    if ((file = open(argv[1], O_RDONLY, 0)) < 0){
        fprintf(stderr, "ERROR: Cannot open file: %s", argv[1]);
        exit(99);
    }
    if ((elf = elf_begin(file, ELF_C_READ, NULL)) == NULL){
        fprintf(stderr, "ERROR: elf_begin() failed, %s", elf_errmsg(-1));
        exit(99);
    }

    glob_vars = parse_data_sec(elf);
    printf("Name\tValue\n");
    find_in_symtab(elf, glob_vars);

    free(glob_vars.var_values);

    elf_end(elf);
    close(file);
    return 0;
}