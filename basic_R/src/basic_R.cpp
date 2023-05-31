#include "basic_R.hpp"

void BASIC_R::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << "\n";
}
void XCHG_RwA::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ax\n";
}