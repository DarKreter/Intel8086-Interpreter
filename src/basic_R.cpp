#include "basic_R.hpp"

#include "logic.hpp"

void BASIC_R::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << regs_16[frame.decoded.reg] << "\n";
}
void XCHG_RwA::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ax\n";
}