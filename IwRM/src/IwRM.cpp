#include "IwRM.hpp"

using namespace std;

void I2RM_BASIC::PrintRM() const
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
        // offset += 2;
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            // offset += 1;
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
            // offset += 2;
            printf("+%x", (int)(frame.decoded.disp.d[0] +
                                (frame.decoded.disp.d[1] << 8)));
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]";
    }
}

uint16_t I2RM_BASIC::GetRM(Binary_t& binary)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        return binary.GetReg(frame.decoded.w, frame.decoded.rm);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        uint16_t addr =
            frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        uint16_t* val = (uint16_t*)&binary.stack[addr];
        if(LOG)
            printf(" ;[%04x]%04x", addr, *val);
        return *val;
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

        if(frame.decoded.w == 1) {
            uint16_t* val = (uint16_t*)&binary.stack[addr];
            if(LOG)
                printf(" ;[%04x]%04x", addr, *val);
            return *val;
        }
        else {
            uint8_t* val = (uint8_t*)&binary.stack[addr];
            if(LOG)
                printf(" ;[%04x]%02x", addr, *val);
            return (uint16_t&)*val;
        }
    }

    return binary.GetReg(frame.decoded.w, frame.decoded.rm);
}
void I2RM_BASIC::SetRM(Binary_t& binary, uint16_t val, bool log)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        binary.GetReg(frame.decoded.w, frame.decoded.rm) = val;
    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        uint16_t addr =
            frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
        uint16_t imm = frame.decoded.disp.d[2] + (frame.decoded.disp.d[3] << 8);
        uint16_t* val = (uint16_t*)&binary.stack[addr];
        if(LOG && log)
            printf(" ;[%04x]%04x", addr, *val);
        *val = imm;
    }
    else // mod == 00/01/10
    {
        int32_t disp = 0;
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
        uint16_t addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        uint16_t prev_val = binary.stack[addr] + (binary.stack[addr + 1] << 8);
        if(LOG && log)
            printf(" ;[%04x]%04x", addr, prev_val);

        binary.stack[addr] = val;
        binary.stack[addr + 1] = val >> 8;
    }
}

void I2RM_BASIC::Read(uint8_t* t)
{
    Command_t::Read(t);

    frame_length = 3;
    if(frame.decoded.w == 1)
        frame_length += 1;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

    if(frame.decoded.mod == 0x03)
        offset = offset;
    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        offset += 2;
    }
    else {
        switch(frame.decoded.mod) {
        case 1:
            offset += 1;
            break;
        case 2:
            offset += 2;
            break;
        }
    }
}

void I2RM_BASIC::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.w != 1 &&
       (frame.decoded.rm != 3 || frame.decoded.mod != 3))
        printf("byte ");

    PrintRM();

    if(frame.decoded.w == 1)
        printf(", %02x%02x", frame.decoded.disp.d[1 + offset],
               frame.decoded.disp.d[0 + offset]);
    else
        printf(", %x", frame.decoded.disp.d[0 + offset]);
}

void I2RM_BASIC_s::Read(uint8_t* t)
{
    Command_t::Read(t);

    frame_length = 3;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        frame_length++;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

    if(frame.decoded.mod == 0x03)
        offset = offset;
    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        offset += 2;
    }
    else {
        switch(frame.decoded.mod) {
        case 1:
            offset += 1;
            break;
        case 2:
            offset += 2;
            break;
        }
    }
}

void I2RM_BASIC_s::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    if(frame.decoded.w != 1 &&
       (frame.decoded.rm != 1 || frame.decoded.mod != 3))
        printf("byte ");

    PrintRM();
    printf(", ");

    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        printf("%02x%02x", frame.decoded.disp.d[1 + offset],
               frame.decoded.disp.d[0 + offset]);
    else {
        union {
            uint8_t u;
            int8_t i;
        } u;
        u.u = frame.decoded.disp.d[0 + offset];
        if(u.i < 0)
            printf("-%x", (int)-u.i);
        else
            printf("%x", (int)u.i);
    }
}

void TEST_IaRM::Execute(Binary_t& binary)
{
    uint16_t dst = GetRM(binary);
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    int16_t val16;
    int8_t val8;

    if(frame.decoded.w) { // 16-bit
        val16 = dst & src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = false;
        binary.CF = false;
    }
    else {
        val8 = (uint8_t)dst & (uint8_t)src;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = false;
        binary.CF = false;
    }
}

void AND_I2RM::Execute(Binary_t& binary)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int16_t val16;
    int8_t val8;
    if(frame.decoded.s) {
        val16 = (int16_t)dst & (int8_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = false;
        binary.CF = false;
        SetRM(binary, val16, false);
    }
    else if(frame.decoded.w) { // 16-bit
        val16 = (int16_t)dst & (int16_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = false;
        binary.CF = false;
        SetRM(binary, val16, false);
    }
    else {
        val8 = (int8_t)dst & (int8_t)src;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = false;
        binary.CF = false;
        SetRM(binary, val8, false);
    }
}
void OR_I2RM::Execute(Binary_t& binary)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int16_t val16;
    int8_t val8;

    if(frame.decoded.w) { // 16-bit
        val16 = dst | src;
        SetRM(binary, val16, false);
        // dst = val16;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = false;
        binary.CF = false;
    }
    else {
        val8 = (uint8_t)dst | (uint8_t)src;
        dst = val8;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = false;
        binary.CF = false;
    }
}

void CMP_IwRM::Execute(Binary_t& binary)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int val;
    int16_t val16;
    int8_t val8;
    // uint16_t tmp;
    if(frame.decoded.s) { // signed
        // printf("!%x, %d!", dst, (int8_t)src);
        val16 = val = (int16_t)dst - (int8_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst < ((uint16_t)(int8_t)src));
    }
    else if(frame.decoded.w) { // 16-bit
        val16 = val = (int16_t)dst - (int16_t)src;
        // printf("\n%d %d %d", dst, src, val16);
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst < src);
    }
    else {
        val8 = val = (int8_t)dst - (int8_t)src;
        // printf("!%x %x!", (int8_t)dst, (int8_t)src);
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val8 != val);
        binary.CF = (int8_t)dst < (int8_t)src;
    }
}

void SUB_IfRM::Execute(Binary_t& binary)
{
    uint16_t src;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        src = frame.decoded.disp.d[0 + offset] +
              (frame.decoded.disp.d[0 + offset + 1] << 8);
    else
        src = frame.decoded.disp.d[0 + offset];
    uint16_t dst = GetRM(binary);
    int val;
    int16_t val16;
    int8_t val8;
    if(frame.decoded.s) { // signed
        val16 = val = (int16_t)dst - (int8_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst < (uint16_t)(int8_t)src);
        SetRM(binary, val16);
    }
    else if(frame.decoded.w) { // 16-bit
        val16 = val = (int16_t)dst - (int16_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst < src);
        SetRM(binary, val16);
    }
    else {
        val8 = val = (int8_t)dst - (int8_t)src;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val8 != val);
        binary.CF = (dst < src);
        SetRM(binary, val8);
    }
}

void ADD_I2RM::Execute(Binary_t& binary)
{
    uint16_t src;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        src = frame.decoded.disp.d[0 + offset] +
              (frame.decoded.disp.d[0 + offset + 1] << 8);
    else
        src = frame.decoded.disp.d[0 + offset];
    uint16_t dst = GetRM(binary);
    int val;
    int16_t val16;
    int8_t val8;

    if(frame.decoded.s) { // signed
        val16 = val = (int16_t)dst + (int8_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst + ((uint16_t)(int8_t)src)) >= 0x10000;
        SetRM(binary, val16, false);
    }
    else if(frame.decoded.w) { // 16-bit
        val16 = val = (int16_t)dst + (int16_t)src;
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = dst + src >= 0x1000;
        SetRM(binary, val16, false);
    }
    else {
        val8 = val = (int8_t)dst + (int8_t)src;
        binary.ZF = (val8 == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val8 != val);
        binary.CF = (int8_t)dst + (int8_t)src >= 0x100;
        SetRM(binary, val8, false);
    }
}

void MOV_I2RM::Execute(Binary_t& binary)
{
    uint16_t src;
    if(frame.decoded.w == 1)
        src = frame.decoded.disp.d[0 + offset] +
              (frame.decoded.disp.d[0 + offset + 1] << 8);
    else
        src = frame.decoded.disp.d[0 + offset];

    // printf("!%x!", src);
    SetRM(binary, src);
}