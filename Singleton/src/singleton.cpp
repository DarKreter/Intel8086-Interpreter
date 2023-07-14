#include "singleton.hpp"

void RET::Execute(Binary_t& binary)
{
    binary.JumpIS(binary.Pop() - frame_length);
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
