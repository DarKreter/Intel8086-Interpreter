#include "rep.hpp"

void REP::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w') << "\n";
}

void CMPS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    std::cout << (frame.decoded.z == 0 ? 'b' : 'w') << "\n";
}