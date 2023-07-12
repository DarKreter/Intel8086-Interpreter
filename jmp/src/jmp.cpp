#include "jmp.hpp"

using namespace std;

void JMP_BASIC::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    // + 2, because actual position is after jump(jump is 2 byte long)
    printf("%04x", (int)(frame.decoded.disp + pos + 2));
}
void JMP_BASIC::Execute(Binary_t& binary, bool)
{
    binary.textPos += (int)(frame.decoded.disp);
    binary.text += (int)(frame.decoded.disp);
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

void JMP_DS::Execute(Binary_t& binary, bool)
{

    uint16_t disp = (frame.decoded.disp_low + (frame.decoded.disp_high << 8));
    binary.textPos += (int)(disp);
    binary.text += (int)(disp);
}

void JMP_DSS::Execute(Binary_t&, bool) { ; }

void JNE::Execute(Binary_t& binary, bool b)
{
    if(!binary.ZF) {
        JMP_BASIC::Execute(binary, b);
    }
}

void JNB::Execute(Binary_t& binary, bool b)
{
    if(!binary.CF) {
        JMP_BASIC::Execute(binary, b);
    }
}