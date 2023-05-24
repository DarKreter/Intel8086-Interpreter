#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include <bitset>

using namespace std;

void MOV_RM2R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    [[maybe_unused]] uint8_t disp;
    std::cout << "mov ";

    // std::cout << "w: " << (int)frame.decoded.w << endl;
    // std::cout << "d: " << (int)frame.decoded.d << endl;
    // std::cout << "mod: " << (int)frame.decoded.mod << endl;
    // std::cout << "reg: " << (int)frame.decoded.reg << endl;
    // std::cout << "r/m: " << (int)frame.decoded.rm << endl;

    if(frame.decoded.d == 0) {        // from reg
        if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
            std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                               : regs_16[frame.decoded.rm]);
        else // mod == 00/01/10
        {
            switch(frame.decoded.mod) {
            case 0:
                disp = 0;
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
            std::cout << "[" << (int)frame.decoded.rm << "]";
        }

        std::cout << ", "
                  << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << std::endl;
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg]);
        if(frame.decoded.mod == 0x03) // if mod == 3, rm is treated like reg
            std::cout << ", "
                      << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                               : regs_16[frame.decoded.rm])
                      << endl;
        else // mod == 00/01/10
        {
            switch(frame.decoded.mod) {
            case 0:
                disp = 0;
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
            std::cout << ", [" << (int)frame.decoded.rm << "]\n";
        }
    }
}

void XOR_RM2R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    [[maybe_unused]] uint8_t disp;
    std::cout << "xor ";
    if(frame.decoded.mod == 0)
        disp = 0;

    if(frame.decoded.d == 0) { // from reg
        if(frame.decoded.mod == 0x03)
            std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                               : regs_16[frame.decoded.rm]);
        std::cout << ", "
                  << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << std::endl;
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg]);
        if(frame.decoded.mod == 0x03)
            std::cout << ", "
                      << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                               : regs_16[frame.decoded.rm])
                      << endl;
    }
}

void ADD_RMwR::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    [[maybe_unused]] uint8_t disp;
    std::cout << "add [";

    if(frame.decoded.mod == 0)
        disp = 0;
    if(frame.decoded.rm == 0)
        std::cout << "bx+si";
    std::cout << "], "
              << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << std::endl;
}

void INT::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << "int " << std::hex << (int)frame.raw[1] << std::endl;
}

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

Command_t::Command_t(uint8_t fl) : frame_length{fl} {}

void Command_t::PrintCommand(size_t pos)
{
    printf("%04lx: ", pos);
    for(uint8_t i = 0; i < frame_length; i++)
        printf("%02x", GetFramePart(i));
    printf("\t");
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

#endif // COMMANDS_DISASSEMBLY