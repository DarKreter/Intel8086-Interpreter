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

uint16_t& RMwR_BASIC::GetRM(Binary_t& binary, bool log)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        return binary.GetReg(frame.decoded.w, frame.decoded.rm);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        uint16_t addr =
            frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        uint16_t* val = (uint16_t*)&binary.stack[addr];
        if(log)
            printf(" ;[%04x]%04x", addr, *val);
        return *val;
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
        // printf("!%d!",frame.decoded.)
        addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        // printf("!%d!", frame.decoded.w);
        if(log) {
            if(frame.decoded.w == 1) {
                uint16_t* val = (uint16_t*)&binary.stack[addr];
                // printf("!%x %x!", binary.stack[addr], binary.stack[addr +
                // 1]);
                printf(" ;[%04x]%04x", addr, *val);
                return *val;
            }
            else {
                uint8_t* val = (uint8_t*)&binary.stack[addr];
                printf(" ;[%04x]%02x", addr, *val);
                return (uint16_t&)*val;
            }
        }
    }
    return binary.GetReg(frame.decoded.w, frame.decoded.rm);
}

void RMwR_BASIC::SetRM(Binary_t& binary, uint16_t val, bool log)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        binary.GetReg(frame.decoded.w, frame.decoded.rm) = val;

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        std::cout << "AAAAAAAAAA!!!\n";
        // uint16_t addr =
        //     frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        // uint16_t* val = (uint16_t*)&binary.stack[addr];
        // if(log)
        //     printf(" ;[%04x]%04x", addr, *val);
        // return *val;
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
        // printf("!%d!",frame.decoded.)
        addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        // printf("!%d!", frame.decoded.w);
        if(log) {
            if(frame.decoded.w == 1) {
                uint16_t* value = (uint16_t*)&binary.stack[addr];
                printf(" ;[%04x]%04x", addr, *value);
                binary.stack[addr] = *value;
                binary.stack[addr + 1] = ((*value) >> 8);
            }
            else {
                uint8_t* value = (uint8_t*)&binary.stack[addr];
                printf(" ;[%04x]%02x", addr, *value);
                binary.stack[addr] = *value;
            }
        }
    }
}

uint16_t RMwR_BASIC::GetRM_addr(Binary_t& binary, bool log)
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
            union {
                uint16_t u;
                int16_t i;
            } u;
            u.u = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
            if(u.i < 0)
                disp = (int)-u.i;
            else
                disp = (int)u.i;
            break;
            break;
        case 0: // disp == 0
        default:
            break;
        }
        uint16_t addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        // uint16_t b = binary.stack[addr] + (binary.stack[addr + 1] >> 8);
        uint16_t* val = (uint16_t*)&binary.stack[addr];
        if(log)
            printf(" ;[%04x]%04x", addr, *val);
        return addr;
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
                                       : regs_16[frame.decoded.reg]);
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
                                           : regs_16[frame.decoded.reg]);
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << ", ";
        PrintRM();
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
}

void XOR_RM2R::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);
    int16_t val;

    if(frame.decoded.w) {
        val = (int16_t)reg ^ (int16_t)rm;
        if(frame.decoded.d == 0)
            rm = val;
        else
            reg = val;
    }
    else {
        val = (int8_t)reg ^ (int8_t)rm;
        if(frame.decoded.d == 0)
            (int8_t&)rm = val;
        else
            (int8_t&)reg = val;
    }

    // printf("!%x!", val);

    if(frame.decoded.w) {
        binary.ZF = (val == 0);
        binary.SF = (val < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        binary.ZF = (((uint8_t)val & 0xff) == 0);
        binary.SF = (((int8_t)val) < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
}

void CMP_RMaR::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);
    int32_t val;
    int16_t val16;
    int16_t val8;

    if(!frame.decoded.d) // to reg
    {
        if(frame.decoded.w) {
            // printf("!%x %x!", rm, reg);
            val16 = val = (int16_t)rm - (int16_t)reg;
            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = (val16 != val);
            binary.CF = (rm < reg);
        }
        else {
            val8 = val = (int8_t)rm - (int8_t)reg;
            binary.ZF = (val8 == 0);
            binary.SF = (val8 < 0);
            binary.OF = (val8 != val);
            binary.CF = (rm < reg);
        }
    }
    else { // from reg
        if(frame.decoded.w) {
            val16 = val = (int16_t)reg - (int16_t)rm;
            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = (val16 != val);
            binary.CF = (reg < rm);
        }
        else {
            val8 = val = (int8_t)rm - (int8_t)reg;
            binary.ZF = (val8 == 0);
            binary.SF = (val8 < 0);
            binary.OF = (val8 != val);
            binary.CF = (reg < rm);
        }
    }
}

void SUB_RM2R::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);
    int32_t val;
    int16_t val16;
    int16_t val8;

    if(!frame.decoded.d) // to reg
    {
        if(frame.decoded.w) {
            val16 = val = (int16_t)rm - (int16_t)reg;
            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = (val16 != val);
            binary.CF = (rm < reg);
            rm = val16;
        }
        else {
            val8 = val = (int8_t)rm - (int8_t)reg;
            binary.ZF = (val8 == 0);
            binary.SF = (val8 < 0);
            binary.OF = (val8 != val);
            binary.CF = (rm & 0xff) < (reg & 0xff);
            rm = val8;
        }
    }
    else { // from reg
        if(frame.decoded.w) {
            val16 = val = (int16_t)reg - (int16_t)rm;
            binary.ZF = (val16 == 0);
            binary.SF = (val16 < 0);
            binary.OF = (val16 != val);
            binary.CF = (reg < rm);
            reg = val16;
        }
        else {
            val8 = val = (int8_t)rm - (int8_t)reg;
            binary.ZF = (val8 == 0);
            binary.SF = (val8 < 0);
            binary.OF = (val8 != val);
            binary.CF = (reg & 0xff) < (rm & 0xff);
            reg = val8;
        }
    }
}

void ADD_RMwR::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);
    int16_t val;
    if(frame.decoded.w)
        val = reg + rm;

    else
        val = (uint8_t)reg + (uint8_t)rm;

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

void MOV_RM2R::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);

    if(frame.decoded.d == 0) {
        if(frame.decoded.w)
            rm = reg;
        else
            (uint8_t&)rm = reg;
    }

    else {
        if(frame.decoded.w)
            reg = rm;
        else
            reg = (uint8_t)rm;
    }
}

void PUSH_RM::Execute(Binary_t& binary, bool log)
{
    uint16_t rm = GetRM(binary, log);
    binary.stack[--binary.sp] = rm >> 8;
    binary.stack[--binary.sp] = rm;
}

void INC_RM::Execute(Binary_t& binary, bool log)
{
    uint16_t& rm = GetRM(binary, log);
    int32_t val;
    int16_t val16;
    int8_t val8;

    if(frame.decoded.w) {
        val16 = val = (int16_t)rm + 1;
        rm = val16;
        // printf("!%x!", val16);
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val != val16);
        binary.CF = binary.CF;
    }
    else {
        val8 = val = (int8_t)rm + 1;
        rm = val8;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val != val8);
        binary.CF = binary.CF;
    }
}

void NEG::Execute(Binary_t& binary, bool log)
{
    uint16_t& rm = GetRM(binary, log);
    int16_t val;
    int16_t val16;
    int8_t val8;

    if(frame.decoded.w) {
        val16 = val = -rm;
        rm = val16;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val != val16);
        binary.CF = (rm != 0);
    }
    else {
        val8 = val = -rm;
        rm = val8;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val != val8);
        binary.CF = (rm != 0);
    }
}

void LEA::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(1, frame.decoded.reg);
    uint16_t rm = GetRM_addr(binary, log);

    reg = rm;
}

void OR_RMaR::Execute(Binary_t& binary, bool log)
{
    uint16_t& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);
    uint16_t& rm = GetRM(binary, log);
    int val;
    int16_t val16;
    int8_t val8;

    if(frame.decoded.w)
        val16 = val = reg | rm;
    else
        val8 = val = (uint8_t)reg | (uint8_t)rm;

    if(frame.decoded.d == 0)
        rm = val;
    else
        reg = val;

    if(frame.decoded.w) { // 16-bit
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
    else {
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = 0;
        binary.CF = 0;
    }
}