#ifndef REP_DIS
#define REP_DIS

#include "commands.hpp"

struct REP : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;

protected:
    // XXXXXXX z(1) XXXXXXX w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t GetFramePart(uint8_t i) const override { return frame.raw[i]; }
    void SetFramePart(uint8_t i, uint8_t val) override { frame.raw[i] = val; }

public:
    void Disassemble(size_t) const override;
    
    REP(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }
    ~REP() = default;
};
struct REP_MOVS : public REP {
    // 1111001 z(1) 1010010 w(1)
    constexpr static std::string_view pattern = "111100101010010";

    REP_MOVS() : REP("rep movs") { ; }
    ~REP_MOVS() = default;
};
struct REP_STOS : public REP {
    // 1111001 z(1) 1010010 w(1)
    constexpr static std::string_view pattern = "111100101010101";

    REP_STOS() : REP("rep stos") { ; }
    ~REP_STOS() = default;
};
struct REP_SCAS : public REP {
    // 1111001 z(1) 1010111 w(1)
    constexpr static std::string_view pattern = "111100101010111";

    // void Execute(Binary_t& ) override;
    REP_SCAS() : REP("rep scas") { ; }
    ~REP_SCAS() = default;
};
struct CMPS : public REP {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    // 1111001 z(1) 1010111 w(1)
    constexpr static std::string_view pattern = "1010011";

    CMPS() : REP("cmps", size_max) { ; }
    void Disassemble(size_t) const override;
    ~CMPS() = default;
};

#endif // REP_DIS