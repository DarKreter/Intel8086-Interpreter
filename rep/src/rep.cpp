#include "rep.hpp"

void REP::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}

void CMPS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    std::cout << (frame.decoded.z == 0 ? 'b' : 'w') << "\n";
}