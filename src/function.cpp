#ifndef FUNCTION_DISASSEMBLY
#define FUNCTION_DISASSEMBLY

#include "function.hpp"
#include "commands.hpp"

#include <bitset>

using namespace std;

void Analyze(uint8_t* tab, size_t size)
{
    size_t pos = 0;
    Command_t* cmd = nullptr;
    while(pos < size) {
        if(CheckPattern(tab, size - pos, "1011"))
            cmd = new MOV_I2R();
        else if(CheckPattern(tab, size - pos, "100010"))
            cmd = new MOV_RM2R();
        else if(CheckPattern(tab, size - pos, "11001101"))
            cmd = new INT();
        else if(CheckPattern(tab, size - pos, "000000"))
            cmd = new ADD_RMwR();
        else if(CheckPattern(tab, size - pos, "001100"))
            cmd = new XOR_RM2R();
        else if(CheckPattern(tab, size - pos, "10001101"))
            cmd = new LEA();
        else {
            cout << pos << ":\t" << std::bitset<8>(*tab) << "\n";
            pos++, tab++;
            continue;
        }

        cmd->Read(tab);
        cmd->PrintCommand(pos);
        pos += cmd->GetFrameLength(), tab += cmd->GetFrameLength();
        delete cmd;
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