#include "basic_R.hpp"

void BASIC_R::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg];
}
void XCHG_RwA::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ax";
}

void PUSH_R::Execute(Binary_t& binary, bool)
{
    uint16_t& reg = binary.GetReg(1, frame.decoded.reg);

    binary.stack[--binary.sp] = reg >> 8;
    binary.stack[--binary.sp] = reg;
}