#ifndef FUNCTION_DISASSEMBLY
#define FUNCTION_DISASSEMBLY

#include "function.hpp"
#include <bitset>

void Analyze(uint8_t* tab, size_t size)
{
    size_t pos = 0;
    while(pos < size) {
        if(CheckPattern(tab, size - pos, "1011")) {
            printf("mov\n");
            // 1101 w(1) reg(3)  data(8/16)
            union {
                uint8_t raw[3];
                struct {
                    uint8_t reg : 3;
                    uint8_t w : 1;
                    uint8_t : 4;
                    uint8_t data[2];
                } decoded;
            } frame;
            frame.raw[0] = *tab;
            frame.raw[1] = *(tab + 1);
            frame.raw[2] = *(tab + 2);
            
            std::cout << std::bitset<8>(frame.decoded.w) << std::endl;
            std::cout << std::bitset<8>(frame.decoded.reg) << std::endl;
            std::cout << std::bitset<8>(frame.decoded.data[0]) << std::endl;
        }
        printf("%02X ", *tab);
        pos++, tab++;
    }
}

bool CheckPattern(uint8_t* tab, size_t sizeLeft, std::string pattern)
{
    for(unsigned int i = 0; i < pattern.length(); i++) {
        if(pattern[i] == 'X')
            continue;
        bool bit = (*tab << (i % 8)) & 0x80;
        if((pattern[i] == '1' && !bit) || (pattern[i] == '0' && bit))
            return false;

        if(i % 8 == 7) {
            if(--sizeLeft == 0)
                return false;
            tab++;
        }
    }
    return true;
}

unsigned char* ReadFile(char* fileName)
{
    FILE* fptr;

    // printf("Reading file %s\n", argv[1]);
    if((fptr = fopen(fileName, "rb")) == NULL) {
        printf("Cannot open file!");
        exit(3);
    }
    fseek(fptr, 0L, SEEK_END);
    int sz = ftell(fptr);
    // printf("%d\n", sz);
    fseek(fptr, 0L, SEEK_SET);

    uint8_t* tab = reinterpret_cast<uint8_t*>(malloc(sz * sizeof(uint8_t)));
    (void)!fread(tab, sizeof(uint8_t) * sz, 1, fptr);

    fclose(fptr);

    // printf("%s", tab);
    return tab;
}

#endif // FUNCTION_DISASSEMBLY