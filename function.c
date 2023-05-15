#ifndef FUNCTION_DISASSEMBLY
#define FUNCTION_DISASSEMBLY

#include "function.h"

unsigned char* ReadFile(char* fileName)
{
    FILE *fptr;

    // printf("Reading file %s\n", argv[1]);
    if ((fptr = fopen(fileName,"rb")) == NULL)
    {
        printf("Cannot open file!");
        exit(3);
    }
    fseek(fptr, 0L, SEEK_END);
    int sz = ftell(fptr);
    // printf("%d\n", sz);
    fseek(fptr, 0L, SEEK_SET);

    unsigned char* tab = malloc(sz * sizeof(unsigned char));
    fread(tab,sizeof(unsigned char) * sz, 1, fptr);

    fclose(fptr);

    // printf("%s", tab);
    return tab;
}

#endif //FUNCTION_DISASSEMBLY