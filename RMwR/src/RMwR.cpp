#include "RMwR.hpp"

void RMwR_BASIC::PrintRM() const
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
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
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]";
    }
}
uint16_t RMwR_BASIC::GetRM(const Binary_t& binary) const
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        return binary.GetReg(frame.decoded.w, frame.decoded.rm);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        uint16_t addr =
            frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
        if(LOG)
            printf(" ;[%04x]%04x", addr, *on_stack);
        return *on_stack;
    }
    else // mod == 00/01/10
    {
        int32_t disp = 0;
        uint16_t addr;

        switch(frame.decoded.mod) {
        case 1:
            disp = frame.decoded.disp.s;
            break;
        case 2:
            disp = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            break;
        case 0: // disp == 0
            disp = 0;
        default:
            break;
        }
        addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        if(frame.decoded.w == 1) {
            uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
            if(LOG)
                printf(" ;[%04x]%04x", addr, *on_stack);
            return *on_stack;
        }
        else {
            uint8_t* on_stack = (uint8_t*)&binary.stack[addr];
            if(LOG)
                printf(" ;[%04x]%02x", addr, *on_stack);
            return (uint16_t)*on_stack;
        }
    }
    return 0xFFFF;
}
void RMwR_BASIC::SetRM(Binary_t& binary, uint16_t val, bool log)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        binary.SetReg(frame.decoded.w, frame.decoded.rm, val);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        uint16_t addr =
            frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
        if(LOG && log)
            printf(" ;[%04x]%04x", addr, *on_stack);
        binary.stack[addr] = val;
        binary.stack[addr + 1] = (val >> 8);
    }
    else // mod == 00/01/10
    {
        int32_t disp = 0;
        uint16_t addr;

        switch(frame.decoded.mod) {
        case 1:
            disp = (int)frame.decoded.disp.s;
            break;
        case 2:
            disp = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            break;
        case 0: // disp == 0
            disp = 0;
        default:
            break;
        }
        addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        if(frame.decoded.w == 1) {
            uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
            if(LOG && log)
                printf(" ;[%04x]%04x", addr, *on_stack);
            binary.stack[addr] = val;
            binary.stack[addr + 1] = ((val) >> 8);
        }
        else {
            uint8_t* on_stack = (uint8_t*)&binary.stack[addr];
            if(LOG && log)
                printf(" ;[%04x]%02x", addr, *on_stack);
            binary.stack[addr] = val;
        }
    }
}
uint16_t RMwR_BASIC::GetRM_addr(const Binary_t& binary) const
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
    }
    else // mod == 00/01/10
    {
        int32_t disp = 0;
        switch(frame.decoded.mod) {
        case 1:
            disp = (int)frame.decoded.disp.s;
            break;
        case 2:
            disp = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            break;
        case 0: // disp == 0
            disp = 0;
        default:
            break;
        }
        uint16_t addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
        if(LOG)
            printf(" ;[%04x]%04x", addr, *on_stack);
        return addr;
    }
    return 0xFFFF;
}

void RMwR_BASIC::Read(uint8_t* t)
{
    Command_t::Read(t);

    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;
}

void RMwR_BASIC::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    PrintRM();
}
void RMwR_BASIC_w::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    PrintRM();

    std::cout << ", "
              << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg]);
}
void RMwR_BASIC_dw::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.d == 0) { // from reg
        PrintRM();
        std::cout << ", "
                  << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg]);
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << ", ";
        PrintRM();
    }
}
void LEA::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << regs_16[frame.decoded.reg] << ", ";

    PrintRM();
}

void XCHG_RMwR::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);

    binary.SetReg(frame.decoded.w, frame.decoded.reg, rm);
    SetRM(binary, reg);
}
void CMP_RMaR::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int32_t real;
    int16_t _16bit;
    int16_t _8bit;

    if(!frame.decoded.d) // to reg
    {
        if(frame.decoded.w) {
            _16bit = real = (int16_t)rm - (int16_t)reg;
            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = (_16bit != real);
            binary.CF = (rm < reg);
        }
        else {
            _8bit = real = (int8_t)rm - (int8_t)reg;
            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = (_8bit != real);
            binary.CF = (rm < reg);
        }
    }
    else { // from reg
        if(frame.decoded.w) {
            _16bit = real = (int16_t)reg - (int16_t)rm;
            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = (_16bit != real);
            binary.CF = (reg < rm);
        }
        else {
            _8bit = real = (int8_t)rm - (int8_t)reg;
            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = (_8bit != real);
            binary.CF = (reg < rm);
        }
    }
}
void TEST_RMwR::Execute(Binary_t& binary)
{
    int16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    int16_t rm = GetRM(binary);
    int32_t real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.w) {
        _16bit = real = rm & reg;
        binary.ZF = (_16bit == 0);
        binary.SF = (_16bit < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        _8bit = real = (int8_t)rm & (int8_t)reg;
        binary.ZF = (_8bit == 0);
        binary.SF = (_8bit < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
}

void SUB_RM2R::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int32_t real;
    int16_t _16bit;
    int16_t _8bit;

    if(!frame.decoded.d) // to reg
    {
        if(frame.decoded.w) {
            _16bit = real = (int16_t)rm - (int16_t)reg;
            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = (_16bit != real);
            binary.CF = (rm < reg);
            SetRM(binary, _16bit);
        }
        else {
            _8bit = real = (int8_t)rm - (int8_t)reg;
            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = (_8bit != real);
            binary.CF = (rm & 0xff) < (reg & 0xff);
            SetRM(binary, _8bit);
        }
    }
    else { // from reg
        if(frame.decoded.w) {
            _16bit = real = (int16_t)reg - (int16_t)rm;
            binary.ZF = (_16bit == 0);
            binary.SF = (_16bit < 0);
            binary.OF = (_16bit != real);
            binary.CF = (reg < rm);
            binary.SetReg(frame.decoded.w, frame.decoded.reg, _16bit);
        }
        else {
            _8bit = real = (int8_t)rm - (int8_t)reg;
            binary.ZF = (_8bit == 0);
            binary.SF = (_8bit < 0);
            binary.OF = (_8bit != real);
            binary.CF = (reg & 0xff) < (rm & 0xff);
            binary.SetReg(frame.decoded.w, frame.decoded.reg, _8bit);
        }
    }
}

void XOR_RM2R::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int16_t value;

    if(frame.decoded.w) {
        value = (int16_t)reg ^ (int16_t)rm;
        binary.ZF = (value == 0);
        binary.SF = (value < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        value = (int8_t)reg ^ (int8_t)rm;
        binary.ZF = (((uint8_t)value & 0xff) == 0);
        binary.SF = (((int8_t)value) < 0);
        binary.OF = 0;
        binary.CF = 0;
    }

    if(frame.decoded.d == 0)
        SetRM(binary, value);
    else
        binary.SetReg(frame.decoded.w, frame.decoded.reg, value);
}
void AND_RMaR::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int16_t val;

    if(frame.decoded.w) {
        val = (int16_t)reg & (int16_t)rm;
        binary.ZF = (val == 0);
        binary.SF = (val < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        val = (int8_t)reg & (int8_t)rm;
        binary.ZF = (((uint8_t)val & 0xff) == 0);
        binary.SF = (((int8_t)val) < 0);
        binary.OF = 0;
        binary.CF = 0;
    }

    if(frame.decoded.d == 0)
        SetRM(binary, val);
    else
        binary.SetReg(frame.decoded.w, frame.decoded.reg, val);
}
void ADD_RMwR::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int16_t real;

    if(frame.decoded.w) {
        real = reg + rm;
        binary.ZF = (real == 0);
        binary.SF = (real < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        real = (uint8_t)reg + (uint8_t)rm;
        binary.ZF = ((real & 0xff) == 0);
        binary.SF = (((int8_t)real) < 0);
        binary.OF = 0;
        binary.CF = 0;
    }

    if(frame.decoded.d == 0)
        SetRM(binary, real, false);
    else
        binary.SetReg(frame.decoded.w, frame.decoded.reg, real);
}
void OR_RMaR::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);
    int real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.w) { // 16-bit
        _16bit = real = reg | rm;
        binary.ZF = (_16bit == 0);
        binary.SF = (_16bit < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        _8bit = real = (uint8_t)reg | (uint8_t)rm;
        binary.ZF = (_8bit == 0);
        binary.SF = (_8bit < 0);
        binary.OF = 0;
        binary.CF = 0;
    }

    if(frame.decoded.d == 0)
        SetRM(binary, real);
    else
        binary.SetReg(1, frame.decoded.reg, real);
}

void MOV_RM2R::Execute(Binary_t& binary)
{
    uint16_t reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t rm = GetRM(binary);

    if(frame.decoded.d == 0) {
        if(frame.decoded.w)
            SetRM(binary, reg, false);
        else
            SetRM(binary, (uint8_t)reg, false);
    }

    else {
        if(frame.decoded.w)
            binary.SetReg(frame.decoded.w, frame.decoded.reg, rm);
        else
            binary.SetReg(frame.decoded.w, frame.decoded.reg, (uint8_t)rm);
    }
}

void PUSH_RM::Execute(Binary_t& binary) { binary.Push(GetRM(binary)); }

void DEC_RM::Execute(Binary_t& binary)
{
    uint16_t rm = GetRM(binary);
    int32_t real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.w) {
        _16bit = real = (int16_t)rm - 1;
        binary.ZF = (_16bit == 0);
        binary.SF = (_16bit < 0);
        binary.OF = (real != _16bit);
        binary.CF = binary.CF;

        SetRM(binary, _16bit, false);
    }
    else {
        _8bit = real = (int8_t)rm - 1;
        binary.ZF = (_8bit == 0);
        binary.SF = (_8bit < 0);
        binary.OF = (real != _8bit);
        binary.CF = binary.CF;

        SetRM(binary, _8bit, false);
    }
}
void INC_RM::Execute(Binary_t& binary)
{
    uint16_t rm = GetRM(binary);
    int32_t real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.w) {
        _16bit = real = (int16_t)rm + 1;
        SetRM(binary, _16bit, false);

        binary.ZF = (_16bit == 0);
        binary.SF = (_16bit < 0);
        binary.OF = (real != _16bit);
        binary.CF = binary.CF;
    }
    else {
        _8bit = real = (int8_t)rm + 1;
        SetRM(binary, _8bit, false);
        binary.ZF = (_8bit == 0);
        binary.SF = (_8bit < 0);
        binary.OF = (real != _8bit);
        binary.CF = binary.CF;
    }
}
void NEG::Execute(Binary_t& binary)
{
    uint16_t rm = GetRM(binary);
    int16_t real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.w) {
        _16bit = real = -rm;
        SetRM(binary, _16bit);
        binary.ZF = (_16bit == 0);
        binary.SF = (_16bit < 0);
        binary.OF = (real != _16bit);
        binary.CF = (rm != 0);
    }
    else {
        _8bit = real = -rm;
        SetRM(binary, _8bit);
        binary.ZF = (_8bit == 0);
        binary.SF = (_8bit < 0);
        binary.OF = (real != _8bit);
        binary.CF = (rm != 0);
    }
}

void LEA::Execute(Binary_t& binary)
{
    uint16_t rm = GetRM_addr(binary);

    binary.SetReg(1, frame.decoded.reg, rm);
}
