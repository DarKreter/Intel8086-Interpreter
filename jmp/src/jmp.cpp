#include "jmp.hpp"

using namespace std;

void JMP_IS::Read(uint8_t* t)
{
    Command_t::Read(t);

    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;
}

void JMP_BASIC::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);
    // + 2, because actual position is after jump(jump is 2 byte long)
    printf("%04x", (int)(frame.decoded.disp + pos + 2));
}
void JMP_DS::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    printf("%04x", (uint16_t)(frame.decoded.disp_low +
                              (frame.decoded.disp_high << 8) + pos + 3));
}
void JMP_IS::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << regs_16[frame.decoded.rm];
}

void JMP_BASIC::Execute(Binary_t& binary) { binary.JumpDS(frame.decoded.disp); }
void JMP_DS::Execute(Binary_t& binary)
{
    binary.JumpDS(frame.decoded.disp_low + (frame.decoded.disp_high << 8));
}
void JMP_IS::Execute(Binary_t& binary)
{
    binary.JumpIS(binary.GetReg(1, frame.decoded.rm) - frame_length);
}
void JNE::Execute(Binary_t& binary)
{
    if(!binary.ZF) {
        JMP_BASIC::Execute(binary);
    }
}
void JB::Execute(Binary_t& binary)
{
    if(binary.CF) {
        JMP_BASIC::Execute(binary);
    }
}
void JNB::Execute(Binary_t& binary)
{
    if(!binary.CF) {
        JMP_BASIC::Execute(binary);
    }
}
void JNBE::Execute(Binary_t& binary)
{
    if(!binary.CF && !binary.ZF) {
        JMP_BASIC::Execute(binary);
    }
}
void JE::Execute(Binary_t& binary)
{
    if(binary.ZF) {
        JMP_BASIC::Execute(binary);
    }
}
void JBE::Execute(Binary_t& binary)
{
    if(binary.CF || binary.ZF) {
        JMP_BASIC::Execute(binary);
    }
}
void JNL::Execute(Binary_t& binary)
{
    if(binary.SF == binary.OF) {
        JMP_BASIC::Execute(binary);
    }
}
void JNLE::Execute(Binary_t& binary)
{
    if(!binary.ZF && (binary.SF == binary.OF)) {
        JMP_BASIC::Execute(binary);
    }
}
void JLE::Execute(Binary_t& binary)
{
    if(binary.ZF || (binary.SF != binary.OF)) {
        JMP_BASIC::Execute(binary);
    }
}
void JL::Execute(Binary_t& binary)
{
    if(binary.SF != binary.OF) {
        JMP_BASIC::Execute(binary);
    }
}
