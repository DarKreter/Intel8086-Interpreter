#include "singleton.hpp"

void Singleton::Disassemble(size_t pos) { Command_t::Disassemble(pos); }

void RET::Execute(Binary_t& binary)
{
    uint16_t pos = binary.stack[binary.sp++];
    pos += (binary.stack[binary.sp++] << 8);
    pos--; // later we add 1 after RET
    // printf("!RET:%x!", pos);
    binary.text += pos - binary.textPos;
    binary.textPos = pos;
}

void CLD::Execute(Binary_t&)
{
    // TODO
}

void CBW::Execute(Binary_t& binary)
{
    uint16_t& src = binary.GetReg(0, 0);
    uint16_t& dst = binary.GetReg(1, 0);

    uint16_t v = (int8_t)src;
    dst = v;
}

void CWD::Execute(Binary_t& binary)
{
    int32_t val = (int16_t)binary.ax;
    binary.dx = (val >> 16);
}
