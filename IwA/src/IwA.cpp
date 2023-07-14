#include "IwA.hpp"

void IwA::Read(uint8_t* t)
{
    Command_t::Read(t);

    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;
}

void IwA::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.w == 1)
        printf("ax, %02x%02x", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("al, %x", frame.decoded.data[0]);
}

int32_t CalculateDifference(int32_t value1, int32_t value2, bool is16bit)
{
    int32_t difference = value1 - value2;
    if(is16bit)
        return static_cast<int16_t>(difference);
    else
        return static_cast<int8_t>(difference);
}

void UpdateBinaryFlags(Binary_t& binary, int32_t difference, bool is16bit)
{
    if(is16bit) {
        binary.ZF = (static_cast<int16_t>(difference) == 0);
        binary.SF = (static_cast<int16_t>(difference) < 0);
        binary.OF = (difference != static_cast<int16_t>(difference));
        binary.CF = (binary.ax < static_cast<uint16_t>(difference));
    }
    else {
        binary.ZF = (static_cast<int8_t>(difference) == 0);
        binary.SF = (static_cast<int8_t>(difference) < 0);
        binary.OF = (difference != static_cast<int8_t>(difference));
        binary.CF = (binary.a.l < static_cast<uint8_t>(difference));
    }
}

void CMP_IwA::Execute(Binary_t& binary)
{
    uint16_t data = frame.decoded.data[0];
    if(frame.decoded.w) {
        data += (static_cast<uint16_t>(frame.decoded.data[1]) << 8);
    }

    int32_t difference =
        CalculateDifference(static_cast<int32_t>(binary.ax),
                            static_cast<int32_t>(data), frame.decoded.w);
    UpdateBinaryFlags(binary, difference, frame.decoded.w);
}

void SUB_IfA::Execute(Binary_t& binary)
{
    uint16_t data = frame.decoded.data[0];

    if(frame.decoded.w) {
        data += (frame.decoded.data[1] << 8);

        uint16_t ax = binary.ax;
        int32_t difference = CalculateDifference(ax, data, true);
        binary.ax = difference;
        UpdateBinaryFlags(binary, difference, true);
    }
    else {
        uint8_t al = binary.a.l;
        int32_t difference = CalculateDifference(al, data, false);
        binary.a.l = difference;
        UpdateBinaryFlags(binary, difference, false);
    }
}
