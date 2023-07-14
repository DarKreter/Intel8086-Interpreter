#include "logic.hpp"

void LGC_BASIC::Read(uint8_t* t)
{
    Command_t::Read(t);

    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;
}

void LGC_BASIC::PrintBase(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);
}
void LGC_BASIC::Disassemble(size_t pos) const
{
    PrintBase(pos);

    printf(", %s", frame.decoded.v == 0 ? "1" : "cl");
}
void DIV::Disassemble(size_t pos) const { PrintBase(pos); }

void DIV::Execute(Binary_t& binary)
{
    if(frame.decoded.w) {
        uint16_t divisor = binary.GetReg(frame.decoded.w, frame.decoded.rm);
        uint32_t dividend =
            (static_cast<uint32_t>(binary.dx) << 16) | binary.ax;
        uint16_t quotient = dividend / divisor;
        uint16_t remainder = dividend % divisor;
        binary.ax = quotient;
        binary.dx = remainder;
    }
    else {
        uint8_t divisor = binary.GetReg(frame.decoded.w, frame.decoded.rm);
        uint8_t quotient = binary.ax / divisor;
        uint8_t remainder = binary.ax % divisor;
        binary.a.l = quotient;
        binary.a.h = remainder;
    }
}
void SAR::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.rm);
    int16_t _16bit;
    int8_t _8bit;

    auto c_rshift = [](uint16_t value, uint16_t count) -> uint8_t {
        return count ? (value >> (count - 1)) & 1 : 0;
    };

    if(frame.decoded.w) {
        if(frame.decoded.v) {
            _16bit = reg >> (binary.c.l & 0x1f);
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _16bit);

            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = binary.OF;
            binary.CF = c_rshift(reg, (binary.c.l & 0x1f));
        }
        else {
            _16bit = reg >> 1;
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _16bit);

            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = false;
            binary.CF = c_rshift(reg, 1);
        }
    }
    else {
        if(frame.decoded.v) {
            _8bit = static_cast<uint8_t>(reg) >> (binary.c.l & 0x1f);
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _8bit);

            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = binary.OF;
            binary.CF = c_rshift(reg, (binary.c.l & 0x1f));
        }
        else {
            _8bit = static_cast<uint8_t>(reg) >> 1;
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _8bit);
            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = false;
            binary.CF = c_rshift(reg, 1);
        }
    }
}
void SHL::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.rm);
    int16_t _16bit;

    auto c_lshift = [](uint16_t value, uint16_t count) -> uint8_t {
        return count ? ((value << (count - 1)) >> 15) & 1 : 0;
    };

    if(frame.decoded.w) {
        if(frame.decoded.v) {
            _16bit = reg << (binary.c.l & 0x1f);
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _16bit);

            uint8_t temp = c_lshift(reg, (binary.c.l & 0x1f));
            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = ((_16bit >> 15) & 1) != temp;
            binary.CF = temp;
        }
        else {
            _16bit = reg << 1;
            binary.SetReg(frame.decoded.w, frame.decoded.rm, _16bit);
            uint8_t temp = c_lshift(reg, 1);

            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = ((_16bit >> 15) & 1) != temp;
            binary.CF = temp;
        }
    }
}
