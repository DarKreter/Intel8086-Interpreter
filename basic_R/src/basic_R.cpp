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

void POP_R::Execute(Binary_t& binary, bool)
{
    uint16_t& reg = binary.GetReg(1, frame.decoded.reg);

    reg = binary.stack[binary.sp++];
    reg += binary.stack[binary.sp++] << 8;
}

void INC_R::Execute(Binary_t& binary, bool)
{
    uint16_t& reg = binary.GetReg(1, frame.decoded.reg);

    reg++;
}

void DEC_R::Execute(Binary_t& binary, bool)
{
    uint16_t& reg = binary.GetReg(1, frame.decoded.reg);
    int16_t val16;
    int32_t val;
    val16 = val = reg - 1;
    reg = val16;

    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (val16 != val);
    binary.CF = binary.CF;
}