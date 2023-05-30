#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
// #include <bitset>

using namespace std;

void MOV_I2R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
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

    printf("%02x%02x\n", frame.decoded.disp_high, frame.decoded.disp_low);
}
void REP_MOVS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}
void REP_STOS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}
void REP_SCAS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}
void CMPS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}
void IN_PORT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("%02x\n", frame.decoded.port);
}
void IN_PORT_VAR::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("dx\n");
}
void CALL_DS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    printf("%04lx\n", frame.decoded.disp + pos + 3);
}
void CALL_IS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    cout << regs_16[frame.decoded.rm] << endl;
}

void INT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << std::hex << (int)frame.raw[1] << std::endl;
}
void HLT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}
void CBW::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}
void CWD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}
void CLD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}
void STD::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}
void RET::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}

void SUB_IfA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

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
        printf("al, ");
    else
        printf("ax, ");

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
        printf("al, ");
    else
        printf("ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
}
void ADD_IwA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
}
void MOV_MwA::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    printf("[%02x%02x]\n", frame.decoded.addr_high, frame.decoded.addr_low);
}

Command_t::Command_t(uint8_t _frame_length, const char* _name)
    : frame_length{_frame_length}, name{_name}
{
}

void Command_t::PrintCommand(size_t pos)
{
    printf("%04lx: ", pos);
    for(uint8_t i = 0; i < frame_length; i++)
        printf("%02x", GetFramePart(i));
    printf("\t\t%s ", name);
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

#endif // COMMANDS_DISASSEMBLY