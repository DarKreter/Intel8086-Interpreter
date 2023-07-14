#ifndef DATA_INT
#define DATA_INT

#include <cstddef>
#include <stdint.h>
#include <string>
#include <vector>

#define TEXT_START_BYTE 32

struct Binary_t {
    static constexpr uint32_t STACK_SIZE = 0x10000;

    size_t textSegmentSize;
    size_t dataSegmentSize;
    uint16_t textPos;
    uint8_t *text, *text_mem_start, *data, *stack;

    // registers
    union {
        uint16_t ax;
        struct {
            int8_t l;
            int8_t h;
        } a;
    };
    union {
        uint16_t bx;
        struct {
            int8_t l;
            int8_t h;
        } b;
    };
    union {
        uint16_t cx;
        struct {
            int8_t l;
            int8_t h;
        } c;
    };
    union {
        uint16_t dx;
        struct {
            int8_t l;
            int8_t h;
        } d;
    };
    uint16_t sp, bp, si, di;
    // flags
    uint8_t OF : 1, DF : 1, SF : 1, ZF : 1, CF : 1;
    uint16_t GetRM_mem(uint8_t rm) const;

    uint16_t GetReg(const uint8_t& w, const uint8_t& reg) const;
    void SetReg(const uint8_t& w, const uint8_t& reg, uint16_t val);

    void PrintStatus();
    void StackInit(std::vector<std::string> argv,
                   std::vector<std::string> envp);

    Binary_t(uint8_t*);
    ~Binary_t();
};

#endif // DATA_INT