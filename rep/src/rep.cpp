#include "rep.hpp"

void REP::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w');
}
void CMPS::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);
    std::cout << (frame.decoded.z == 0 ? 'b' : 'w');
}
