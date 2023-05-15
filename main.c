#include "function.h"

#define TEXT_START_BYTE 32

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("No command-line arguments!\n");
        exit(1);
    }
    else if(argc > 2)
    {
        printf("Too many command-line arguments!\n");
        exit(2);
    }

    unsigned char* fileContent = ReadFile(argv[1]);

    int textSegmentSize = fileContent[8];

    for(int i = TEXT_START_BYTE; i < TEXT_START_BYTE + textSegmentSize; i++)
    {   
        printf("%02X\n", fileContent[i]);
    }
    return 0;
}