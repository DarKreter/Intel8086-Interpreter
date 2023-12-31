#ifndef FUNCTION_DISASSEMBLY
#define FUNCTION_DISASSEMBLY

#include "function.hpp"
#include "IwA.hpp"
#include "IwRM.hpp"
#include "RMwR.hpp"
#include "basic_R.hpp"
#include "commands.hpp"
#include "jmp.hpp"
#include "logic.hpp"
#include "rep.hpp"
#include "singleton.hpp"

using namespace std;

void Execute(Binary_t& binary)
{
    Command_t* cmd = nullptr;
    if(LOG)
        printf(" AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP\n");
    while(binary.textPos < binary.textSegmentSize) {
        cmd = CheckAllCommands<
            MOV_I2R, MOV_RM2R, OR_I2RM, INT, ADD_RMwR, ADD_I2RM, AND_I2RM,
            MOV_I2RM, XOR_RM2R, SUB_RM2R, LEA, CMP_IwRM, CMP_RMaR, TEST_IaRM,
            INC_RM, JNB, JLE, JB, JNE, JNBE, JBE, JL, JNLE, JNL, JE, JS, JMP_DS,
            JMP_IS, DIV, XCHG_RMwR, XCHG_RwA, LOOP, JMP_DSS, MOV_MwA, RET_wSAI,
            PUSH_R, ADD_IwA, SAR, RCL, INC_R, PUSH_RM, DEC_RM, MUL, CALL_DS,
            CALL_IS, HLT, CBW, CWD, DEC_R, ADC_RMwR, TEST_RMwR, SHL, SHR,
            REP_MOVS, REP_STOS, REP_SCAS, CMPS, POP_R, AND_RMaR, IN_PORT,
            IN_PORT_VAR, SBB_RMaR, SUB_IfRM, SUB_IfA, CMP_IwA, OR_RMaR, NEG,
            SSB_I2RM, RET, CLD, STD, TEST_IwA>(
            binary.text, binary.textSegmentSize - binary.textPos);
        if(cmd != nullptr)
            ;
        else {
            printf("%04x: %02x            (undefined)\n", binary.textPos,
                   *binary.text);
            binary.textPos++, binary.text++;
            continue;
        }
        cmd->Read(binary.text);
        if(LOG)
            cmd->PrintStatus(binary);
        try {
            cmd->Execute(binary);
        }
        catch(std::runtime_error& e) {
            break;
        }
        catch(int exit) {
            delete cmd;
            throw exit;
        }

        binary.textPos += cmd->GetFrameLength(),
            binary.text += cmd->GetFrameLength();
        delete cmd;
        cmd = nullptr;
        if(LOG)
            std::cout << std::endl;
    }
}

void Analyze(Binary_t& binary)
{
    Command_t* cmd = nullptr;

    while(binary.textPos < binary.textSegmentSize) {
        cmd = CheckAllCommands<
            MOV_I2R, MOV_RM2R, OR_I2RM, INT, ADD_RMwR, ADD_I2RM, AND_I2RM,
            MOV_I2RM, XOR_RM2R, SUB_RM2R, LEA, CMP_IwRM, CMP_RMaR, TEST_IaRM,
            INC_RM, JNB, JLE, JB, JNE, JNBE, JBE, JL, JNLE, JNL, JE, JS, JMP_DS,
            JMP_IS, DIV, XCHG_RMwR, XCHG_RwA, LOOP, JMP_DSS, MOV_MwA, RET_wSAI,
            PUSH_R, ADD_IwA, SAR, RCL, INC_R, PUSH_RM, DEC_RM, MUL, CALL_DS,
            CALL_IS, HLT, CBW, CWD, DEC_R, ADC_RMwR, TEST_RMwR, SHL, SHR,
            REP_MOVS, REP_STOS, REP_SCAS, CMPS, POP_R, AND_RMaR, IN_PORT,
            IN_PORT_VAR, SBB_RMaR, SUB_IfRM, SUB_IfA, CMP_IwA, OR_RMaR, NEG,
            SSB_I2RM, RET, CLD, STD, TEST_IwA>(
            binary.text, binary.textSegmentSize - binary.textPos);
        if(cmd != nullptr)
            ;
        else {
            printf("%04x: %02x            (undefined)\n", binary.textPos,
                   *binary.text);
            binary.textPos++, binary.text++;
            continue;
        }

        cmd->Read(binary.text);
        cmd->Disassemble(binary.textPos);
        binary.textPos += cmd->GetFrameLength(),
            binary.text += cmd->GetFrameLength();
        delete cmd;
        cmd = nullptr;
        std::cout << std::endl;
    }
}

template <typename... Ts>
Command_t* CheckAllCommands(uint8_t* tab, size_t diff)
{
    Command_t* cmd = nullptr;
    // Iterate over each type and call CheckCommand
    ((CheckCommand<Ts>(tab, diff) == true ? cmd = new Ts() : cmd), ...);
    return cmd;
}

template <typename T>
bool CheckCommand(uint8_t* tab, size_t diff)
{
    if(T::size_min <= diff && CheckPattern(tab, diff, T::pattern))
        return true;
    return false;
}

bool CheckPattern(uint8_t* tab, size_t sizeLeft, std::string_view pattern)
{
    for(unsigned int i = 0; i < pattern.length(); i++) {
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

    if((fptr = fopen(fileName, "rb")) == NULL) {
        printf("Cannot open file!");
        exit(3);
    }
    fseek(fptr, 0L, SEEK_END);
    int sz = ftell(fptr);

    fseek(fptr, 0L, SEEK_SET);

    uint8_t* tab = reinterpret_cast<uint8_t*>(malloc(sz * sizeof(uint8_t)));
    (void)!fread(tab, sizeof(uint8_t) * sz, 1, fptr);

    fclose(fptr);

    return tab;
}

#endif // FUNCTION_DISASSEMBLY