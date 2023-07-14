#include "singleton.hpp"

void RET::Execute(Binary_t& binary)
{
    uint16_t pos = binary.stack[binary.sp++];
    pos += (binary.stack[binary.sp++] << 8);
    pos--; // later we add 1 after RET

    binary.text += pos - binary.textPos;
    binary.textPos = pos;
}

void CLD::Execute(Binary_t&)
{
    // TODO
}

void CBW::Execute(Binary_t& binary)
{
    int8_t src = binary.GetReg(0, 0);

    uint16_t v = src;
    binary.SetReg(1, 0, v);
}

void CWD::Execute(Binary_t& binary)
{
    int32_t val = (int16_t)binary.ax;
    binary.dx = (val >> 16);
}
