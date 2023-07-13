#include "IwA.hpp"

void IwA::Disassemble(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x", frame.decoded.data[0]);
}

void CMP_IwA::Execute(Binary_t& binary, bool)
{
    uint16_t data;
    int32_t val;
    int16_t val16;
    int8_t val8;
    if(frame.decoded.w)
        data = frame.decoded.data[0] + (frame.decoded.data[1] << 8);
    else
        data = frame.decoded.data[0];

    if(frame.decoded.w == 0) {
        val8 = val = (int8_t)binary.a.l - (int8_t)data;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val != val8);
        binary.CF = binary.a.l < (data & 0xff);
    }
    else {
        val16 = val = (int16_t)binary.ax - (int16_t)data;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val != val16);
        binary.CF = binary.ax < data;
    }
}