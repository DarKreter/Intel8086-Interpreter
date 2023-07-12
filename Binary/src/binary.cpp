#include "binary.hpp"
#include "message.hpp"
#include <cstdio>
#include <iostream>

uint16_t& Binary_t::GetReg(uint8_t w, uint8_t reg)
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
            return reinterpret_cast<uint16_t&>(a.l);
        case 1:
            return reinterpret_cast<uint16_t&>(c.l);
        case 2:
            return reinterpret_cast<uint16_t&>(d.l);
        case 3:
            return reinterpret_cast<uint16_t&>(b.l);
        case 4:
            return reinterpret_cast<uint16_t&>(a.h);
        case 5:
            return reinterpret_cast<uint16_t&>(c.h);
        case 6:
            return reinterpret_cast<uint16_t&>(d.h);
        case 7:
            return reinterpret_cast<uint16_t&>(b.h);
        }
    }
    return ax;
}

void Binary_t::PrintStatus()
{
    printf("%04x %04x %04x %04x %04x %04x %04x %04x %c%c%c%c ", ax, bx, cx, dx,
           sp, bp, si, di, ((OF) ? 'O' : '-'), ((SF) ? 'S' : '-'),
           ((ZF) ? 'Z' : '-'), ((CF) ? 'C' : '-'));
}

void Binary_t::StackInit(std::vector<std::string> argv,
                         std::vector<std::string> envp)
{
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

    // for(uint16_t i = sp; i != 0; i++)
    //     printf("%d ", (int)stack[i]);
    // printf("\n");
}

Binary_t::Binary_t(uint8_t* fileContent)
    : textPos{}, ax{}, bx{}, cx{}, dx{}, sp{uint16_t(0xffd6)}, bp{}, si{}, di{},
      OF{}, DF{}, SF{}, ZF{}, CF{}
{

    textSegmentSize = (fileContent[11] << 24) + (fileContent[10] << 16) +
                      (fileContent[9] << 8) + fileContent[8];
    dataSegmentSize = (fileContent[15] << 24) + (fileContent[14] << 16) +
                      (fileContent[13] << 8) + fileContent[12];

    text = new uint8_t[textSegmentSize];
    for(size_t i = 0; i < textSegmentSize; i++)
        text[i] = fileContent[i + TEXT_START_BYTE];

    data = new uint8_t[dataSegmentSize];
    for(size_t i = 0; i < dataSegmentSize; i++)
        data[i] = fileContent[i + TEXT_START_BYTE + textSegmentSize];

    stack = new uint8_t[0xffff];
}

Binary_t::~Binary_t()
{
    text -= textPos;
    delete[] text;
    delete[] data;
    delete[] stack;
}