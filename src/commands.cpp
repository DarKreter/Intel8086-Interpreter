#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include <bitset>

void MOV_I2R::PrintCommand()
{
    std::cout << (int)frame_length << std::endl;
    std::cout << std::bitset<8>(frame.decoded.w) << std::endl;
    std::cout << std::bitset<8>(frame.decoded.reg) << std::endl;
    std::cout << std::bitset<8>(frame.decoded.data[0]) << std::endl;
    std::cout << std::bitset<8>(frame.decoded.data[1]) << std::endl;
}

Command_t::Command_t(uint8_t fl) : frame_length{fl} {}

void Command_t::Read(uint8_t* tab)
{
    for(int i = 0; i < frame_length; i++)
        GetFramePart(i) = *(tab++);
}

#endif // COMMANDS_DISASSEMBLY