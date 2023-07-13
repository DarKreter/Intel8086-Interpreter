#include "singleton.hpp"

void Singleton::Disassemble(size_t pos) { Command_t::Disassemble(pos); }

void RET::Execute(Binary_t& binary, bool)
{
    uint16_t pos = binary.stack[binary.sp++];
    pos += (binary.stack[binary.sp++] << 8);
    pos--; // later we add 1 after RET
    // printf("!RET:%x!", pos);
    binary.text += pos - binary.textPos;
    binary.textPos = pos;
}