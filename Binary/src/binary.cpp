#include "binary.hpp"
#include "message.hpp"
#include <cstdio>

int16_t& Binary_t::GetReg(uint8_t w, uint8_t reg)
{
    if(w == 0) {
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
            return reinterpret_cast<int16_t&>(a.l);
        case 1:
            return reinterpret_cast<int16_t&>(c.l);
        case 2:
            return reinterpret_cast<int16_t&>(d.l);
        case 3:
            return reinterpret_cast<int16_t&>(b.l);
        case 4:
            return reinterpret_cast<int16_t&>(a.h);
        case 5:
            return reinterpret_cast<int16_t&>(c.h);
        case 6:
            return reinterpret_cast<int16_t&>(d.h);
        case 7:
            return reinterpret_cast<int16_t&>(b.h);
        }
    }
    return ax;
}

void Binary_t::PrintStatus()
{
    printf("%04x %04x %04x %04x %04x %04x %04x %04x ", ax, bx, cx, dx, sp, bp,
           si, di);
    printf("---- ");
}

Binary_t::Binary_t(uint8_t* fileContent)
    : textPos{}, ax{}, bx{}, cx{}, dx{}, sp{}, bp{}, si{}, di{}
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
}

Binary_t::~Binary_t()
{
    text -= textPos;
    delete[] text;
    delete[] data;
}