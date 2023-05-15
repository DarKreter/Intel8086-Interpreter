#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include <bitset>

void MOV_I2R::PrintCommand(size_t pos)
{
    printf("%04lX: ", pos);
    for(uint8_t i = 0; i < frame_length; i++)
        printf("%02X", frame.raw[i]);
    std::cout << "\t\tmov " << regs_16[frame.decoded.reg] << ", ";

    printf("%02X", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02X", frame.decoded.data[0]);

    printf("\n");
}

Command_t::Command_t(uint8_t fl) : frame_length{fl} {}

void Command_t::Read(uint8_t* tab)
{
    for(int i = 0; i < frame_length; i++)
        GetFramePart(i) = *(tab++);
}

std::map<uint8_t, std::string> regs_8 = {{0, "AL"}, {1, "CL"}, {2, "DL"},
                                         {3, "BL"}, {4, "AH"}, {5, "CH"},
                                         {6, "DH"}, {7, "BH"}};
std::map<uint8_t, std::string> regs_16 = {{0, "AX"}, {1, "CX"}, {2, "DX"},
                                          {3, "BX"}, {4, "SP"}, {5, "BP"},
                                          {6, "SI"}, {7, "DI"}};

#endif // COMMANDS_DISASSEMBLY