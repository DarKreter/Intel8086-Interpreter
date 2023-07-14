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
    binary.Push(binary.GetReg(1, frame.decoded.reg));
}
void POP_R::Execute(Binary_t& binary)
{
    binary.SetReg(1, frame.decoded.reg, binary.Pop());
}

void INC_R::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(1, frame.decoded.reg);
    int16_t val16 = reg + 1;

    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (reg == INT16_MAX);

    binary.SetReg(1, frame.decoded.reg, val16);
}

void DEC_R::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(1, frame.decoded.reg);
    int16_t val16 = reg - 1;

    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (reg == INT16_MIN);

    binary.SetReg(1, frame.decoded.reg, val16);
}
