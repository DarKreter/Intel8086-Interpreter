#include "RMwR.hpp"

void RMwR_BASIC::PrintRM()
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
            union {
                uint16_t u;
                int16_t i;
            } u;
            u.u = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            if(u.i < 0)
                printf("-%x", (int)-u.i);
            else
                printf("+%x", (int)u.i);
            break;
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]";
    }
}

uint16_t& RMwR_BASIC::GetRM(Binary_t& binary)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        return binary.GetReg(frame.decoded.w, frame.decoded.rm);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
            union {
                uint16_t u;
                int16_t i;
            } u;
            u.u = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            if(u.i < 0)
                printf("-%x", (int)-u.i);
            else
                printf("+%x", (int)u.i);
            break;
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]";
    }
    return binary.GetReg(frame.decoded.w, frame.decoded.rm);
}

void RMwR_BASIC::Disassemble(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    PrintRM();
    std::cout << std::endl;
}

void RMwR_BASIC_w::Disassemble(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    PrintRM();

    std::cout << ", "
              << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << std::endl;
}

void RMwR_BASIC_dw::Disassemble(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    if(frame.decoded.d == 0) { // from reg
        PrintRM();
        std::cout << ", "
                  << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << std::endl;
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << ", ";
        PrintRM();
        std::cout << std::endl;
    }
}

void LEA::Disassemble(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ";

    PrintRM();
    std::cout << std::endl;
}

void XOR_RM2R::Execute(Binary_t& binary, bool)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary);
    int16_t val;
    if(frame.decoded.w)
        val = reg ^ rm;

    else
        val = (uint8_t)reg ^ (uint8_t)rm;

    if(frame.decoded.d == 0)
        rm = val;
    else
        reg = val;

    if(frame.decoded.w) {
        binary.ZF = (val == 0);
        binary.SF = (val < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        binary.ZF = ((val & 0xff) == 0);
        binary.SF = (((int8_t)val) < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
}

void MOV_RM2R::Execute(Binary_t& binary, bool)
{

    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary);

    if(frame.decoded.d == 0)
        rm = reg;
    else
        reg = rm;
}