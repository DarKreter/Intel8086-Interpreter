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

    printf("%02x", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02x", frame.decoded.data[0]);

    printf("\n");
}
void RET_wSAI::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("ret %02x%02x\n", frame.decoded.disp_high, frame.decoded.disp_low);
}
void PUSH_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "push " << regs_16[frame.decoded.reg] << "\n";
}
void REP_MOV::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "rep movs" << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
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
void XCHG_RwA::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "xchg " << regs_16[frame.decoded.reg] << ", ax\n";
}
void INC_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "inc " << regs_16[frame.decoded.reg] << "\n";
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

    printf(", %s\n", frame.decoded.v == 0 ? "1" : "cl");
}
void SHR::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);
    std::cout << "shr ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    printf(", %s\n", frame.decoded.v == 0 ? "1" : "cl");
}
void SAR::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);
    std::cout << "sar ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    printf(", %s\n", frame.decoded.v == 0 ? "1" : "cl");
}
void RCL::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);
    std::cout << "rcl ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    std::cout << ", " << (int)(frame.decoded.v == 0 ? 1 : 69) << std::endl;
}
void DIV::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);
    std::cout << "div ";

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);
    printf("\n");
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
void CBW::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("cbw\n");
}
void CWD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("cwd\n");
}
void CLD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("cld\n");
}
void STD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("std\n");
}
void RET::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("ret\n");
}
void SUB_IfA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("sub al, ");
    else
        printf("sub ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
}
void CMP_IwA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("cmp al, ");
    else
        printf("cmp ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
}
void TEST_IwA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("test al, ");
    else
        printf("test ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
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