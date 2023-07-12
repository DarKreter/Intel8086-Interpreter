#include "IwRM.hpp"

using namespace std;

void I2RM_BASIC::PrintRM()
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
        offset += 2;
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            offset += 1;
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
            offset += 2;
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
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
    }
    else // mod == 00/01/10
    {
        int32_t disp = 0;
        switch(frame.decoded.mod) {
        case 1:
            if(frame.decoded.disp.s < 0)
                disp = (int)-frame.decoded.disp.s;
            else
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

        uint16_t val = frame.decoded.disp.d[0 + offset] +
                       (frame.decoded.disp.d[0 + offset + 1] << 8);
        printf(" ;[%04x]%04x", addr, val);
        return (uint16_t)(binary.stack[addr] + (binary.stack[addr + 1] << 8));
    }
    return binary.GetReg(frame.decoded.w, frame.decoded.rm);
}
void I2RM_BASIC::SetRM(Binary_t& binary, uint16_t val)
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        binary.GetReg(frame.decoded.w, frame.decoded.rm) = val;
    else
        std::cout << "AAAAAAAAAA!!!\n";
    // else if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
    // {
    //     printf("[%02x%02x]", frame.decoded.disp.d[1],
    //     frame.decoded.disp.d[0]);
    // }
    // else // mod == 00/01/10
    // {
    //     int32_t disp = 0;
    //     switch(frame.decoded.mod) {
    //     case 1:
    //         if(frame.decoded.disp.s < 0)
    //             disp = (int)-frame.decoded.disp.s;
    //         else
    //             disp = (int)frame.decoded.disp.s;
    //         break;
    //     case 2:
    //         union {
    //             uint16_t u;
    //             int16_t i;
    //         } u;
    //         u.u = frame.decoded.disp.d[0] + (frame.decoded.disp.d[1] << 8);
    //         if(u.i < 0)
    //             disp = (int)-u.i;
    //         else
    //             disp = (int)u.i;
    //         break;
    //         break;
    //     case 0: // disp == 0
    //     default:
    //         break;
    //     }
    //     uint16_t addr = binary.GetRM_mem(frame.decoded.rm) + disp;

    //     uint16_t val = frame.decoded.disp.d[0 + offset] +
    //                    (frame.decoded.disp.d[0 + offset + 1] << 8);
    //     printf(" ;[%04x]%04x", addr, val);
    //     return (uint16_t)(binary.stack[addr] + (binary.stack[addr + 1] <<
    //     8));
    // }
    // return binary.GetReg(frame.decoded.w, frame.decoded.rm);
}

void I2RM_BASIC::Disassemble(size_t pos)
{
    frame_length = 3;
    if(frame.decoded.w == 1)
        frame_length += 1;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

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

void I2RM_BASIC_s::Disassemble(size_t pos)
{
    frame_length = 3;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        frame_length++;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

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

void TEST_IaRM::Execute(Binary_t& binary, bool)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] >> 8);
    uint16_t dst = GetRM(binary);
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

void CMP_IwRM::Execute(Binary_t& binary, bool)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int val;
    int16_t val16;
    int8_t val8;

    // if(frame.decoded.w) { // signed
    // val16 = val = (int16)dst - (char)src;
    //     binary.ZF = (val16 == 0);
    //     binary.SF = (val16 < 0);
    //     binary.OF = (val16 != val);
    //     binary.CF = (dst < (char)src);
    // }
    if(frame.decoded.w) { // 16-bit
        val16 = val = (uint16_t)dst - (uint16_t)src;
        // printf("\n%d %d %d", dst, src, val16);
        binary.ZF = (val16 == 0);
        binary.SF = (val16 < 0);
        binary.OF = (val16 != val);
        binary.CF = (dst < src);
    }
    else {
        val8 = val = (uint8_t)dst - (uint8_t)src;
        binary.ZF = ((val8 & 0xff) == 0);
        binary.SF = (val8 < 0);
        binary.OF = (val8 != val);
        binary.CF = (dst < src);
    }
}

void SUB_IfRM::Execute(Binary_t& binary, bool)
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
    // int8_t val8;

    // if(frame.decoded.w) { // signed
    val16 = val = (int16_t)dst - (int8_t)src;
    SetRM(binary, val16);
    binary.ZF = (val16 == 0);
    binary.SF = (val16 < 0);
    binary.OF = (val16 != val);
    binary.CF = (dst < (int8_t)src);

    // }
    // if(frame.decoded.w) { // 16-bit
    //     val16 = val = (uint16_t)dst - (uint16_t)src;
    //     SetRM(binary, val16);
    //     binary.ZF = (val16 == 0);
    //     binary.SF = (val16 < 0);
    //     binary.OF = (val16 != val);
    //     binary.CF = (dst < src);
    // }
    // else {
    //     val8 = val = (uint8_t)dst - (uint8_t)src;
    //     SetRM(binary, val8);
    //     binary.ZF = (val8 == 0);
    //     binary.SF = (val8 < 0);
    //     binary.OF = (val8 != val);
    //     binary.CF = (dst < src);
    // }
}
