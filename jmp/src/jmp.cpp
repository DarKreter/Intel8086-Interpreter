#include "jmp.hpp"

using namespace std;

void JMP_BASIC::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    // + 2, because actual position is after jump(jump is 2 byte long)
    printf("%04x", (int)(frame.decoded.disp + pos + 2));
}
void JMP_DS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    printf("%04x", (uint16_t)(frame.decoded.disp_low +
                                (frame.decoded.disp_high << 8) + pos + 3));
}
void JMP_IS::Disassemble(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << regs_16[frame.decoded.rm];
}
