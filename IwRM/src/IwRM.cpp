#include "IwRM.hpp"

using namespace std;

void I2RM_BASIC::PrintRM() const
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
        uint16_t* on_stack = (uint16_t*)&binary.stack[addr];
        if(LOG)
            printf(" ;[%04x]%04x", addr, *on_stack);
        return *on_stack;
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
            return *on_stack;
        }
    }

    return 0xFFFF; // error
}
void I2RM_BASIC::SetRM(Binary_t& binary, uint16_t val, bool log)
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
        *on_stack = val;
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
        case 0:
            disp = 0;
        default:
            break;
        }
        uint16_t addr = binary.GetRM_mem(frame.decoded.rm) + disp;
        uint16_t on_stack = binary.stack[addr] + (binary.stack[addr + 1] << 8);
        if(LOG && log)
            printf(" ;[%04x]%04x", addr, on_stack);

        binary.stack[addr] = val;
        binary.stack[addr + 1] = val >> 8;
    }
}

void I2RM_BASIC::Read(uint8_t* t)
{
    Command_t::Read(t);

    frame_length = 3;
    if(frame.decoded.w)
        frame_length++;

    if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        offset += 2;
        frame_length += 2;
    }
    else if(frame.decoded.mod < 3) {
        offset += frame.decoded.mod;
        frame_length += frame.decoded.mod;
    }
}
void I2RM_BASIC_s::Read(uint8_t* t)
{
    Command_t::Read(t);

    frame_length = 3;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        frame_length++;

    if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        frame_length += 2;
        offset += 2;
    }
    else if(frame.decoded.mod < 3) {
        offset += frame.decoded.mod;
        frame_length += frame.decoded.mod;
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

template <typename T>
void inline static SetFlags_1(Binary_t& binary, T value)
{
    binary.ZF = (value == 0);
    binary.SF = (value < 0);
    binary.OF = 0;
    binary.CF = 0;
}
template <typename T_1, typename T_2, typename T_3, typename T_4>
void inline static SetFlags_2(Binary_t& binary, T_1 _bit, T_2 value, T_3 dst,
                              T_4 src)
{
    binary.ZF = (_bit == 0);
    binary.SF = (_bit < 0);
    binary.OF = (_bit != value);
    binary.CF = (dst < ((uint16_t)(int8_t)src));
}
template <typename T_1, typename T_2, typename T_3, typename T_4>
void inline static SetFlags_3(Binary_t& binary, T_1 _bit, T_2 real, T_3 dst,
                              T_4 src)
{
    binary.ZF = (_bit == 0);
    binary.SF = (_bit < 0);
    binary.OF = (_bit != real);
    binary.CF = (dst + src) >= 0x10000;
}

void MOV_I2RM::Execute(Binary_t& binary)
{
    uint16_t src = frame.decoded.disp.d[0 + offset];
    if(frame.decoded.w == 1)
        src += (frame.decoded.disp.d[0 + offset + 1] << 8);

    SetRM(binary, src);
}
void TEST_IaRM::Execute(Binary_t& binary)
{
    uint16_t dst = GetRM(binary);
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);

    if(frame.decoded.w) { // 16-bit
        int16_t _16bit = dst & src;
        SetFlags_1(binary, _16bit);
    }
    else {
        int8_t _8bit = (uint8_t)dst & (uint8_t)src;
        SetFlags_1(binary, _8bit);
    }
}
void CMP_IwRM::Execute(Binary_t& binary)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int real;
    int16_t _16bit;
    int8_t _8bit;

    if(frame.decoded.s) { // signed
        _16bit = real = (int16_t)dst - (int8_t)src;
        SetFlags_2(binary, _16bit, real, dst, src);
    }
    else if(frame.decoded.w) { // 16-bit
        _16bit = real = (int16_t)dst - (int16_t)src;
        SetFlags_2(binary, _16bit, real, dst, src);
    }
    else {
        _8bit = real = (int8_t)dst - (int8_t)src;
        SetFlags_2(binary, _8bit, real, (int8_t)dst, (int8_t)src);
    }
}
void I2RM_BASIC::_I2RM(Binary_t& binary, auto& op)
{
    uint16_t src = frame.decoded.disp.d[0 + offset] +
                   (frame.decoded.disp.d[0 + offset + 1] << 8);
    uint16_t dst = GetRM(binary);
    int16_t _16bit;
    int8_t _8bit;
    if(frame.decoded.s) {
        _16bit = op((int16_t)dst, (int8_t)src);
        SetFlags_1(binary, _16bit);

        SetRM(binary, _16bit, false);
    }
    else if(frame.decoded.w) { // 16-bit
        _16bit = op((int16_t)dst, (int16_t)src);
        SetFlags_1(binary, _16bit);

        SetRM(binary, _16bit, false);
    }
    else {
        _8bit = op((int8_t)dst, (int8_t)src);
        SetFlags_1(binary, _8bit);

        SetRM(binary, _8bit, false);
    }
}
void AND_I2RM::Execute(Binary_t& binary)
{
    auto lmb = [](auto x, auto y) { return x & y; };
    _I2RM(binary, lmb);
}
void OR_I2RM::Execute(Binary_t& binary)
{
    auto lmb = [](auto x, auto y) { return x | y; };
    _I2RM(binary, lmb);
}
void SUB_IfRM::Execute(Binary_t& binary)
{
    auto lmb = [](auto x, auto y) { return x - y; };
    _I2RM(binary, lmb);
}
void ADD_I2RM::Execute(Binary_t& binary)
{
    auto lmb = [](auto x, auto y) { return x + y; };
    _I2RM(binary, lmb);
}
