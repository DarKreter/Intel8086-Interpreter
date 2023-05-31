#ifndef DATA_INT
#define DATA_INT

#include <stdint.h>
#include <cstddef>

#define TEXT_START_BYTE 32

struct Binary_t {
    size_t textSegmentSize;
    size_t dataSegmentSize;
    size_t textPos;
    uint8_t* text;
    uint8_t* data;

    Binary_t(uint8_t*);
    ~Binary_t();
};

#endif // DATA_INT