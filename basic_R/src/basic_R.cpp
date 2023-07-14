#include "basic_R.hpp"

void BASIC_R::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg];
}
void XCHG_RwA::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ax";
}

void XCHG_RwA::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(1, frame.decoded.reg);

    binary.SetReg(1, frame.decoded.reg, binary.ax);
    binary.ax = reg;
}

void PUSH_R::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(1, frame.decoded.reg);

    binary.stack[--binary.sp] = reg >> 8;
    binary.stack[--binary.sp] = reg;
}

void POP_R::Execute(Binary_t& binary)
{
    uint16_t value;

    value = binary.stack[binary.sp++];
    value += binary.stack[binary.sp++] << 8;

    binary.SetReg(1, frame.decoded.reg, value);
}

void INC_R::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(1, frame.decoded.reg);
    int32_t val;
    int16_t val16;

    val16 = val = reg + 1;
    binary.SetReg(1, frame.decoded.reg, val16);

    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (val != val16);
    binary.CF = binary.CF;
}

void DEC_R::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(1, frame.decoded.reg);
    int16_t val16;
    int32_t val;
    val16 = val = reg - 1;

    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (val16 != val);
    binary.CF = binary.CF;

    binary.SetReg(1, frame.decoded.reg, val16);
}