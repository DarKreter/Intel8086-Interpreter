#ifndef DATA_INT
#define DATA_INT

#include <cstddef>
#include <stdint.h>

#define TEXT_START_BYTE 32

struct Binary_t {
    size_t textSegmentSize;
    size_t dataSegmentSize;
    size_t textPos;
    uint8_t* text;
    uint8_t* data;

    union {
        int16_t ax;
        struct {
            int8_t l;
            int8_t h;
        } a;
    };
    union {
        int16_t bx;
        struct {
            int8_t l;
            int8_t h;
        } b;
    };
    union {
        int16_t cx;
        struct {
            int8_t l;
            int8_t h;
        } c;
    };
    union {
        int16_t dx;
        struct {
            int8_t l;
            int8_t h;
        } d;
    };
    int16_t sp, bp, si, di;
    int16_t& GetReg(uint8_t w, uint8_t reg);

    void PrintStatus();
    Binary_t(uint8_t*);
    ~Binary_t();
};



#endif // DATA_INT