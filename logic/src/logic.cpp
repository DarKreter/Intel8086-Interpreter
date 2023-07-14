#include "logic.hpp"

void LGC_BASIC::PrintBase(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);
}
void LGC_BASIC::Disassemble(size_t pos)
{
    PrintBase(pos);

    printf(", %s", frame.decoded.v == 0 ? "1" : "cl");
}
void DIV::Disassemble(size_t pos) { PrintBase(pos); }

void DIV::Execute(Binary_t& binary)
{
    int32_t val;
    if(frame.decoded.w) {
        uint16_t num = binary.GetReg(frame.decoded.w, frame.decoded.rm);
        uint32_t numerator = (uint32_t)(binary.dx << 16 | binary.ax);
        uint16_t quot = val = numerator / num;
        uint16_t rem = numerator % num;
        binary.ax = quot;
        binary.dx = rem;
    }
    else {
        uint8_t num = binary.GetReg(frame.decoded.w, frame.decoded.rm);
        uint8_t quot = (uint16_t)binary.ax / num;
        uint8_t rem = (uint16_t)binary.ax % num;
        binary.a.l = quot;
        binary.a.h = rem;
    }
}
void SAR::Execute(Binary_t& binary)
{
    int16_t& reg = (int16_t&)binary.GetReg(frame.decoded.w, frame.decoded.rm);
    int32_t val;
    int16_t val16;
    int8_t val8;
    auto c_rshift = [](uint16_t value, uint16_t count) -> uint8_t {
        return count ? (value >> (count - 1)) & 1 : 0;
    };
    if(frame.decoded.w) {
        if(frame.decoded.v) {
            if(binary.c.l) {
                // printf("!%d!", reg);
                val16 = val = reg >> (binary.c.l & 0x1f);
                binary.ZF = (val16 == 0);
                binary.SF = (val16 < 0);
                binary.OF = binary.OF;
                binary.CF = c_rshift(reg, (binary.c.l & 0x1f));

                reg = val16;
            }
        }
        else {
            val16 = val = reg >> 1;
            reg = val16;

            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = false;
            binary.CF = c_rshift(reg, 1);
        }
    }
    else {
        if(frame.decoded.v) {
            if(binary.c.l) {
                val8 = val = (uint8_t)reg >> (binary.c.l & 0x1f);
                (uint8_t&)reg = val8;

                binary.ZF = (val8 == 0);
                binary.SF = (val8 < 0);
                binary.OF = binary.OF;
                binary.CF = c_rshift(reg, (binary.c.l & 0x1f));
            }
        }
        else {
            val8 = val = (uint8_t)reg >> 1;
            (uint8_t&)reg = val8;
            binary.ZF = (val8 == 0);
            binary.SF = (val8 < 0);
            binary.OF = false;
            binary.CF = c_rshift(reg, 1);
        }
    }
}
void SHL::Execute(Binary_t& binary)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.rm);
    int16_t val16;
    int32_t val;

    if(frame.decoded.w) {
        auto c_lshift = [](uint16_t value, uint16_t count) -> uint8_t {
            return count ? ((value << (count - 1)) >> 15) & 1 : 0;
        };

        if(frame.decoded.v) {
            if(binary.c.l) {
                val16 = val = (reg << (binary.c.l & 0x1f));

                uint8_t carry = c_lshift(reg, (binary.c.l & 0x1f));
                binary.ZF = (val16 == 0);
                binary.SF = (val16 < 0);
                binary.OF = ((val16 >> 15) & 1) != carry;
                binary.CF = carry;

                reg = val16;
            }
        }
        else {
            val16 = val = (reg << 1);
            reg = val16;
            uint8_t carry = c_lshift(reg, 1);

            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = ((val16 >> 15) & 1) != carry;
            binary.CF = carry;
        }
    }
    else {
        std::cout << "AAAAAAAAAA - SHL\n";
        // auto c_lshift = [](uint8_t value, uint8_t count) -> uint8_t {
        //     return count ? ((value << (count - 1)) >> 7) & 1 : 0;
        // };
        // uchar dst = readEA(&opcode);
        // if(opcode.v) {
        //     if(CL) {
        //         val8 = val = (dst << (CL & 0x1f));
        //         ;
        //         writeEA(&opcode, val8);
        //         // for making the same as 7run
        //         uchar carry = c_lshift(dst, 1);
        //         setZSOC((val8 == 0), (val8 < 0), ((val8 >> 7) & 1) != carry,
        //                 carry);
        //     }
        // }
        // else {
        //     val8 = val = (dst << 1);
        //     writeEA(&opcode, val8);
        //     uchar carry = c_lshift(dst, 1);
        //     setZSOC((val8 == 0), (val8 < 0), ((val8 >> 7) & 1) != carry,
        //     carry);
        // }
    }
}