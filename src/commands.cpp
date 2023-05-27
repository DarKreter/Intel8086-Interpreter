#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include <bitset>

using namespace std;

void MOV_I2R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "mov "
              << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << ", ";

    printf("%02X", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02X", frame.decoded.data[0]);

    printf("\n");
}
void CMP_IwRM::PrintCommand(size_t pos)
{
    // std::cout << "w: " << (int)frame.decoded.w << endl;
    // std::cout << "s: " << (int)frame.decoded.s << endl;
    // std::cout << "mod: " << (int)frame.decoded.mod << endl;
    // std::cout << "reg: " << (int)frame.decoded.reg << endl;
    // std::cout << "r/m: " << (int)frame.decoded.rm << endl;

    frame_length = 3;
    if((frame.decoded.s == 0 && frame.decoded.w == 1))
        frame_length++;
    if(frame.decoded.mod != 0x03 && frame.decoded.mod != 0x00)
        frame_length++;
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length += 2;

    Command_t::PrintCommand(pos);
    uint8_t offset = 0;
    std::cout << "cmp ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm])
                  << ", ";
    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x], ", frame.decoded.data[1], frame.decoded.data[0]);
        offset = 2;
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            std::cout << "+" << (int)frame.decoded.data[0];
            offset = 1;
            break;
        case 2:
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "], ";
    }

    if(frame.decoded.s == 0 && frame.decoded.w == 1) {
        printf("%02X", frame.decoded.data[1 + offset]);
        printf("%02X\n", frame.decoded.data[0 + offset]);
    }
    else {
        printf("%02X\n", frame.decoded.data[0 + offset]);
    }
}
void TEST_IaRM::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 4;
    else
        frame_length = 3;

    Command_t::PrintCommand(pos);

    std::cout << "test ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm])
                  << ", ";

    if(frame.decoded.w == 1) {
        printf("%02X", frame.decoded.data[1]);
        printf("%02X\n", frame.decoded.data[0]);
    }
    else {
        printf("%02X\n", frame.decoded.data[0]);
    }
}
void PUSH_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "push " << regs_16[frame.decoded.reg] << "\n";
}
void NEG::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 1)
        frame_length = 3;
    else if(frame.decoded.mod == 2)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    cout << "neg ";
    if(frame.decoded.mod == 0x03)
        std::cout << regs_16[frame.decoded.rm] << "\n";
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            std::cout << "+" << (int)frame.decoded.disp[0];
            break;
        case 2:
            std::cout << "+"
                      << (int)(frame.decoded.disp[0] +
                               (frame.decoded.disp[1] << 8));
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]\n";
    }
}
void DEC_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "dec " << regs_16[frame.decoded.reg] << "\n";
}
void POP_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "pop " << regs_16[frame.decoded.reg] << "\n";
}
void IN_PORT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "in " << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("%02x\n", frame.decoded.port);
}
void IN_PORT_VAR::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "in " << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("dx\n");
}
void CALL_DS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    printf("call %04lx\n", frame.decoded.disp + pos + 3);
}
void CALL_IS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    cout << "call " << regs_16[frame.decoded.rm] << endl;
}
void SHL::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);
    std::cout << "shl ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    std::cout << ", " << (int)(frame.decoded.v == 0 ? 1 : 69) << std::endl;
}
void INT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "int " << std::hex << (int)frame.raw[1] << std::endl;
}
void HLT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("hlt\n");
}
void RET::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("ret\n");
}

Command_t::Command_t(uint8_t fl) : frame_length{fl} {}

void Command_t::PrintCommand(size_t pos)
{
    printf("%04lx: ", pos);
    for(uint8_t i = 0; i < frame_length; i++)
        printf("%02x", GetFramePart(i));
    printf("\t\t");
}

void Command_t::Read(uint8_t* tab)
{
    for(int i = 0; i < frame_length; i++)
        GetFramePart(i) = *(tab++);
}

std::map<uint8_t, std::string> regs_8 = {{0, "al"}, {1, "cl"}, {2, "dl"},
                                         {3, "bl"}, {4, "ah"}, {5, "ch"},
                                         {6, "dh"}, {7, "bh"}};
std::map<uint8_t, std::string> regs_16 = {{0, "ax"}, {1, "cx"}, {2, "dx"},
                                          {3, "bx"}, {4, "sp"}, {5, "bp"},
                                          {6, "si"}, {7, "di"}};
std::map<uint8_t, std::string> rm_memory = {
    {0, "bx+si"}, {1, "bx+di"}, {2, "bp+si"}, {3, "bp+di"},
    {4, "si"},    {5, "di"},    {6, "bp"},    {7, "bx"}};

#endif // COMMANDS_DISASSEMBLYregs_8