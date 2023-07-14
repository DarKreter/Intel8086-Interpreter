#ifndef BASICR_DIS
#define BASICR_DIS

#include "commands.hpp"

struct BASIC_R : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;

protected:
    // 01000 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t GetFramePart(uint8_t i) const override { return frame.raw[i]; }
    void SetFramePart(uint8_t i, uint8_t val) override { frame.raw[i] = val; }
    void PrintBase(size_t pos);
    
public:
    void Disassemble(size_t) const override;

    BASIC_R(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }
    ~BASIC_R() = default;
};

struct INC_R : public BASIC_R {
    // 01000 reg(3)
    constexpr static std::string_view pattern = "01000";

    void Execute(Binary_t&) override;
    INC_R() : BASIC_R("inc") { ; }
    ~INC_R() = default;
};
struct POP_R : public BASIC_R {
    // 01011 reg(3)
    constexpr static std::string_view pattern = "01011";
    void Execute(Binary_t&) override;

    POP_R() : BASIC_R("pop") { ; }
    ~POP_R() = default;
};
struct DEC_R : public BASIC_R {
    // 01001 reg(3)
    constexpr static std::string_view pattern = "01001";

    void Execute(Binary_t&) override;
    
    DEC_R() : BASIC_R("dec") { ; }
    ~DEC_R() = default;
};
struct PUSH_R : public BASIC_R {
    // 01010 reg(3)
    constexpr static std::string_view pattern = "01010";

    void Execute(Binary_t&) override;

    PUSH_R() : BASIC_R("push") { ; }
    ~PUSH_R() = default;
};
struct XCHG_RwA : public BASIC_R {
    // 10010 reg(3)
    constexpr static std::string_view pattern = "10010";

    void Disassemble(size_t) const override;
    void Execute(Binary_t&) override;

    XCHG_RwA() : BASIC_R("xchg") { ; }
    ~XCHG_RwA() = default;
};

#endif // BASICR_DIS