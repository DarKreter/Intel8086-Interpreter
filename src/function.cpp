#ifndef FUNCTION_DISASSEMBLY
#define FUNCTION_DISASSEMBLY

#include "function.hpp"
#include "commands.hpp"

#include <bitset>

using namespace std;

void Analyze(uint8_t* tab, size_t size)
{
    // cout << hex << size << endl;
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
        else if(CheckPattern(tab, size - pos, "100000XXXX000"))
            cmd = new ADD_I2RM();
        else if(CheckPattern(tab, size - pos, "001100"))
            cmd = new XOR_RM2R();
        else if(CheckPattern(tab, size - pos, "10001101"))
            cmd = new LEA();
        else if(CheckPattern(tab, size - pos, "100000XXXX111"))
            cmd = new CMP_IwRM();
        else if(CheckPattern(tab, size - pos, "1111011XXX000"))
            cmd = new TEST_IaRM();
        else if(CheckPattern(tab, size - pos, "01110011"))
            cmd = new JNB();
        else if(CheckPattern(tab, size - pos, "01110101"))
            cmd = new JNE();
        else if(CheckPattern(tab, size - pos, "01111100"))
            cmd = new JL();
        else if(CheckPattern(tab, size - pos, "01110100"))
            cmd = new JE();
        else if(CheckPattern(tab, size - pos, "11101001"))
            cmd = new JMP_DS();
        else if(CheckPattern(tab, size - pos, "11101011"))
            cmd = new JMP_DSS();
        else if(CheckPattern(tab, size - pos, "01010"))
            cmd = new PUSH_R();
        else if(CheckPattern(tab, size - pos, "11111111XX110"))
            cmd = new PUSH_RM();
        else if(CheckPattern(tab, size - pos, "11101000"))
            cmd = new CALL_DS();
        else if(CheckPattern(tab, size - pos, "11111111XX010"))
            cmd = new CALL_IS();
        else if(CheckPattern(tab, size - pos, "11110100"))
            cmd = new HLT();
        else if(CheckPattern(tab, size - pos, "01001"))
            cmd = new DEC_R();
        else if(CheckPattern(tab, size - pos, "110100XXXX100"))
            cmd = new SHL();
        else if(CheckPattern(tab, size - pos, "01011"))
            cmd = new POP_R();
        else if(CheckPattern(tab, size - pos, "001000"))
            cmd = new AND_RMaR();
        else if(CheckPattern(tab, size - pos, "1110010"))
            cmd = new IN_PORT();
        else if(CheckPattern(tab, size - pos, "1110110"))
            cmd = new IN_PORT_VAR();
        else if(CheckPattern(tab, size - pos, "000110"))
            cmd = new SBB_RMaR();
        else if(CheckPattern(tab, size - pos, "100000XXXX101"))
            cmd = new SUB_IfRM();
        // else if(CheckPattern(tab, size - pos, "000010"))
        //     cmd = new OR_RMaR();
        else {
            cout << hex << pos << ":\t" << std::bitset<8>(*tab) << "\n";
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
        bool bit = (*tab << (i % 8)) & 0x80;
        // if(pattern[i] == 'X') {
        // std::cout << (int)bit << "\n";
        // }
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