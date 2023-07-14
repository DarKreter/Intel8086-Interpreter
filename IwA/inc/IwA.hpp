#ifndef IWA_DIS
#define IWA_DIS

#include "commands.hpp"

struct IwA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;

protected:
    // XXXXXXX w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    IwA(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }
    void PrintBase(size_t pos);

public:
    void Disassemble(size_t) override;
    ~IwA() = default;
};
struct SUB_IfA : public IwA {
    // 0010110 w(1) data(8) (if w == 1)data(8)
    constexpr static std::string_view pattern = "0010110";

    void Execute(Binary_t&) override;
    SUB_IfA() : IwA("sub") { ; }
    ~SUB_IfA() = default;
};
struct CMP_IwA : public IwA {
    // 0011110 w(1) data(8) (if w == 1)data(8)
    constexpr static std::string_view pattern = "0011110";

    void Execute(Binary_t&) override;
    CMP_IwA() : IwA("cmp") { ; }
    ~CMP_IwA() = default;
};
struct TEST_IwA : public IwA {
    // 1010100 w(1) data(8) (if w == 1)data(8)
    constexpr static std::string_view pattern = "1010100";

    TEST_IwA() : IwA("test") { ; }
    ~TEST_IwA() = default;
};
struct ADD_IwA : public IwA {
    // 0000010 w(1) data(8) (if w == 1)data(8)
    constexpr static std::string_view pattern = "0000010";

    ADD_IwA() : IwA("add") { ; }
    ~ADD_IwA() = default;
};

#endif // IWA_DIS