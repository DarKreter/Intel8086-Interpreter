#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
// #include <bitset>

using namespace std;

void Command_t::Execute(Binary_t&) { ; }

void MOV_I2R::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << ", ";

    printf("%02x", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02x", frame.decoded.data[0]);

    printf("\n");
}

void MOV_I2R::Execute(Binary_t& binary)
{
    auto& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);

    if(frame.decoded.w == 0)
        reg = frame.decoded.data[0];
    else
        reg = frame.decoded.data[0] + (frame.decoded.data[1] << 8);
}

void RET_wSAI::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    printf("%02x%02x\n", frame.decoded.disp_high, frame.decoded.disp_low);
}
void IN_PORT::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("%02x\n", frame.decoded.port);
}
void IN_PORT_VAR::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("dx\n");
}
void CALL_DS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    printf("%04lx\n", frame.decoded.disp + pos + 3);
}
void CALL_IS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    cout << regs_16[frame.decoded.rm] << endl;
}

void INT::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << std::hex << (int)frame.raw[1] << std::endl;
}
void INT::Execute(Binary_t& binary)
{
    if(binary.bx == 0x0010)
        throw std::runtime_error("exit");
    else if(binary.bx == 0) {
        printf("<write(1, %04x, %lx)", frame.decoded.type,
               binary.dataSegmentSize - frame.decoded.type);
        for(size_t i = frame.decoded.type; i < binary.dataSegmentSize; i++)
            printf("%c", binary.data[i]);
        printf("=> %lx>\n", binary.dataSegmentSize - frame.decoded.type);
    }
}
void INT::PrintStatus(Binary_t& bin)
{
    Command_t::PrintStatus(bin);
    if(bin.bx == 0x0010)
        printf("<exit(0)>\n");
    // else if(binary.bx == 0)
    //     printf("<\n");
}

void MOV_MwA::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

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

void Command_t::PrintStatus(Binary_t& binary)
{
    binary.PrintStatus();
    Disassemble(binary.textPos);
}

void Command_t::Disassemble(size_t pos)
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