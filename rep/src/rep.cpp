#include "rep.hpp"

void REP::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? 'b' : 'w');
}

void CMPS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    std::cout << (frame.decoded.z == 0 ? 'b' : 'w');
}

// void REP_SCAS::Execute(Binary_t& binary)
// {
//     uint8_t dst, src;

//     if(!binary.cx)
//         return;
// }
