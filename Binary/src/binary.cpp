#include "binary.hpp"
#include "message.hpp"
#include <cstdio>
#include <iostream>

void Binary_t::PrintStatus() const
{
    printf("%04x %04x %04x %04x %04x %04x %04x %04x %c%c%c%c ", ax, bx, cx, dx,
           sp, bp, si, di, ((OF) ? 'O' : '-'), ((SF) ? 'S' : '-'),
           ((ZF) ? 'Z' : '-'), ((CF) ? 'C' : '-'));
}

uint16_t Binary_t::GetRM_mem(uint8_t rm) const
{
    switch(rm) {
    case 0:
        return bx + si;
    case 1:
        return bx + di;
    case 2:
        return bp + si;
    case 3:
        return bp + di;
    case 4:
        return si;
    case 5:
        return di;
    case 6:
        return bp;
    case 7:
        return bx;
    default:
        return 0;
    }
}
uint16_t Binary_t::GetReg(const uint8_t& w, const uint8_t& reg) const
{
    if(w == 1) {
        switch(reg) {
        case 0:
            return ax;
        case 1:
            return cx;
        case 2:
            return dx;
        case 3:
            return bx;
        case 4:
            return sp;
        case 5:
            return bp;
        case 6:
            return si;
        case 7:
            return di;
        }
    }
    else {
        switch(reg) {
        case 0:
            return a.l;
        case 1:
            return c.l;
        case 2:
            return d.l;
        case 3:
            return b.l;
        case 4:
            return a.h;
        case 5:
            return c.h;
        case 6:
            return d.h;
        case 7:
            return b.h;
        }
    }
    return 0xFFFF;
}
void Binary_t::SetReg(const uint8_t& w, const uint8_t& reg, const uint16_t& val)
{
    if(w == 1) {
        switch(reg) {
        case 0:
            ax = val;
            break;
        case 1:
            cx = val;
            break;
        case 2:
            dx = val;
            break;
        case 3:
            bx = val;
            break;
        case 4:
            sp = val;
            break;
        case 5:
            bp = val;
            break;
        case 6:
            si = val;
            break;
        case 7:
            di = val;
            break;
        }
    }
    else {
        switch(reg) {
        case 0:
            a.l = static_cast<uint8_t>(val);
            break;
        case 1:
            c.l = static_cast<uint8_t>(val);
            break;
        case 2:
            d.l = static_cast<uint8_t>(val);
            break;
        case 3:
            b.l = static_cast<uint8_t>(val);
            break;
        case 4:
            a.h = static_cast<uint8_t>(val);
            break;
        case 5:
            c.h = static_cast<uint8_t>(val);
            break;
        case 6:
            d.h = static_cast<uint8_t>(val);
            break;
        case 7:
            b.h = static_cast<uint8_t>(val);
        }
    }
}

void Binary_t::StackInit(const std::vector<std::string>& argv,
                         const std::vector<std::string>& envp)
{
    for(size_t i = 0; i < dataSegmentSize; i++)
        stack[i] = data[i];

    std::vector<uint16_t> argv_addr;
    uint16_t env_addr;
    //          argc + arg_addresses   + NULL + env_addr + NULL
    uint16_t len = 2 + 2 * argv.size() + 2;
    // env_addr + NULL
    len += 2 + 2;

    for(auto& arg : argv) {
        argv_addr.push_back(len);
        len += arg.size() + 1;
    }

    env_addr = len;
    for(auto& env : envp)
        len += env.size() + 1;
    if(len % 2)
        len++;

    sp = STACK_SIZE - len;
    uint16_t sp_cp = sp;

    uint16_t argc = argv.size();
    // argc
    stack[sp++] = argc;
    stack[sp++] = argc >> 8;

    // argv addresses
    for(auto& addr : argv_addr) {
        addr = sp_cp + addr;
        stack[sp++] = addr;
        stack[sp++] = addr >> 8;
    }

    // NULL
    stack[sp++] = 0;
    stack[sp++] = 0;

    // env addr
    env_addr = sp_cp + env_addr;
    stack[sp++] = env_addr;
    stack[sp++] = env_addr >> 8;

    // NULL
    stack[sp++] = 0;
    stack[sp++] = 0;

    // argv
    for(auto& arg : argv) {
        for(auto& c : arg)
            stack[sp++] = c;
        stack[sp++] = '\0';
    }

    // envp
    for(auto& env : envp) {
        for(auto& c : env)
            stack[sp++] = c;
        stack[sp++] = '\0';
    }

    // fullfil with zeros
    while(sp != 0)
        stack[sp++] = 0;
    sp = sp - len;
}

void Binary_t::Push(uint16_t value)
{
    stack[--sp] = value >> 8;
    stack[--sp] = value;
}
uint16_t Binary_t::Pop()
{
    uint16_t temp = stack[sp++];
    return temp | (stack[sp++] << 8);
}
void Binary_t::JumpDS(int16_t disp)
{
    uint8_t* start = text - textPos; // Start of allocated memory
    textPos += (int)(disp);
    text = start + textPos;
}
void Binary_t::JumpIS(int16_t disp)
{
    auto* begin = text - textPos;
    textPos = disp;
    text = begin + textPos;
}

Binary_t::Binary_t(uint8_t* fileContent)
    : textPos{}, ax{}, bx{}, cx{}, dx{}, sp{}, bp{}, si{}, di{}, OF{}, DF{},
      SF{}, ZF{}, CF{}
{

    textSegmentSize = (fileContent[11] << 24) + (fileContent[10] << 16) +
                      (fileContent[9] << 8) + fileContent[8];
    dataSegmentSize = (fileContent[15] << 24) + (fileContent[14] << 16) +
                      (fileContent[13] << 8) + fileContent[12];

    text_mem_start = text = new uint8_t[textSegmentSize];
    for(size_t i = 0; i < textSegmentSize; i++)
        text[i] = fileContent[i + TEXT_START_BYTE];

    data = new uint8_t[dataSegmentSize];
    for(size_t i = 0; i < dataSegmentSize; i++)
        data[i] = fileContent[i + TEXT_START_BYTE + textSegmentSize];

    stack = new uint8_t[STACK_SIZE];
    for(uint32_t i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;
}

Binary_t::~Binary_t()
{
    delete[] text_mem_start;
    delete[] data;
    delete[] stack;
}