#ifndef LOGIC_DIS
#define LOGIC_DIS

#include "commands.hpp"

struct LGC_BASIC : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;

protected:
    // 110100 v(1)w(1) mod(2) 010 r/m(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t v : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t GetFramePart(uint8_t i) const override { return frame.raw[i]; }
    void SetFramePart(uint8_t i, uint8_t val) override { frame.raw[i] = val; }

    void PrintBase(size_t pos) const;
    void Read(uint8_t*) override;

public:
    void Disassemble(size_t) const override;

    LGC_BASIC(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }
    ~LGC_BASIC() = default;
};
struct RCL : public LGC_BASIC {
    // 110100 v(1)w(1) mod(2) 010 r/m(3)
    constexpr static std::string_view pattern = "110100XXXX010";

    RCL() : LGC_BASIC("rcl") { ; }
    ~RCL() = default;
};
struct SHR : public LGC_BASIC {
    // 110100 v(1)w(1) mod(2) 101 r/m(3)
    constexpr static std::string_view pattern = "110100XXXX101";

    SHR() : LGC_BASIC("shr") { ; }
    ~SHR() = default;
};
struct SHL : public LGC_BASIC {
    // 110100v(1)w(1) mod(2)100r/m(3)
    constexpr static std::string_view pattern = "110100XXXX100";

    void Execute(Binary_t&) override;
    SHL() : LGC_BASIC("shl") { ; }
    ~SHL() = default;
};
struct SAR : public LGC_BASIC {
    // 110100 v(1)w(1) mod(2) 111 r/m(3)
    constexpr static std::string_view pattern = "110100XXXX111";

    void Execute(Binary_t&) override;
    SAR() : LGC_BASIC("sar") { ; }
    ~SAR() = default;
};
struct DIV : public LGC_BASIC {
    // 1111011 w(1) mod(2) 110 r/m(3)
    constexpr static std::string_view pattern = "1111011XXX110";

    void Disassemble(size_t) const override;
    void Execute(Binary_t&) override;

    DIV() : LGC_BASIC("div") { ; }
    ~DIV() = default;
};

#endif // LOGIC_DIS