#ifndef RMWR_DIS
#define RMWR_DIS

#include "commands.hpp"

struct RMwR_BASIC : public Command_t {
    constexpr static size_t size_max = 4;
    constexpr static size_t size_min = 2;

protected:
    // XXXXXX d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    union {
        uint8_t raw[size_max];
        struct __attribute__((packed)) {
            uint8_t w : 1;
            uint8_t d : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
            union {
                uint8_t d[2]; // double
                int8_t s;     // single
            } disp;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    void PrintRM();

public:
    RMwR_BASIC(const char* _name) : Command_t(size_max, _name) { ; }

    void PrintCommand(size_t) override;
    ~RMwR_BASIC() = default;
};

struct NEG : public RMwR_BASIC {
    // 1111011 w(1) mod(2) 011 r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1111011XXX011";

    NEG() : RMwR_BASIC("neg") { ; }
    void PrintCommand(size_t) override;
    ~NEG() = default;
};
struct ADC_RMwR : public RMwR_BASIC {
    // 000100 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "000100";

    ADC_RMwR() : RMwR_BASIC("adc") { ; }
    ~ADC_RMwR() = default;
};
struct ADD_RMwR : public RMwR_BASIC {
    // 000000 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "000000";

    ADD_RMwR() : RMwR_BASIC("add") { ; }
    ~ADD_RMwR() = default;
};
struct SUB_RM2R : public RMwR_BASIC {
    // 001010 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "001010";

    SUB_RM2R() : RMwR_BASIC("sub") { ; }
    ~SUB_RM2R() = default;
};
struct CMP_RMaR : public RMwR_BASIC {
    // 001110 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "001110";

    CMP_RMaR() : RMwR_BASIC("cmp") { ; }
    ~CMP_RMaR() = default;
};
struct XOR_RM2R : public RMwR_BASIC {
    // 001100 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "001100";

    XOR_RM2R() : RMwR_BASIC("xor") { ; }
    ~XOR_RM2R() = default;
};
struct SBB_RMaR : public RMwR_BASIC {
    // 000110 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "000110";

    SBB_RMaR() : RMwR_BASIC("sbb") { ; }
    ~SBB_RMaR() = default;
};
struct XCHG_RMwR : public RMwR_BASIC {
    // 1000011 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1000011";

    XCHG_RMwR() : RMwR_BASIC("xchg") { ; }
    void PrintCommand(size_t) override;
    ~XCHG_RMwR() = default;
};
struct OR_RMaR : public RMwR_BASIC {
    // 000010d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "000010";

    OR_RMaR() : RMwR_BASIC("or") { ; }
    ~OR_RMaR() = default;
};
struct MOV_RM2R : public RMwR_BASIC {
    // 100010 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "100010";

    MOV_RM2R() : RMwR_BASIC("mov") { ; }
    ~MOV_RM2R() = default;
};
struct AND_RMaR : public RMwR_BASIC {
    // 001000 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "001000";

    AND_RMaR() : RMwR_BASIC("and") { ; }
    ~AND_RMaR() = default;
};
struct LEA : public RMwR_BASIC {
    // 10001101 mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "10001101";

    LEA() : RMwR_BASIC("lea") { ; }
    void PrintCommand(size_t) override;
    ~LEA() = default;
};
struct PUSH_RM : public RMwR_BASIC {
    // 11111111 mod(2) 110 r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "11111111XX110";

    PUSH_RM() : RMwR_BASIC("push") { ; }
    void PrintCommand(size_t) override;
    ~PUSH_RM() = default;
};
struct DEC_RM : public RMwR_BASIC {
    // 11111111 mod(2) 001 r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1111111XXX001";

    DEC_RM() : RMwR_BASIC("dec") { ; }
    void PrintCommand(size_t) override;
    ~DEC_RM() = default;
};
struct INC_RM : public RMwR_BASIC {
    // 11111111 mod(2) 000 r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1111111XXX000";

    INC_RM() : RMwR_BASIC("inc") { ; }
    void PrintCommand(size_t) override;
    ~INC_RM() = default;
};
struct MUL : public RMwR_BASIC {
    // 1111011 w(1) mod(2) 100 r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1111011XXX100";

    MUL() : RMwR_BASIC("mul") { ; }
    void PrintCommand(size_t) override;
    ~MUL() = default;
};
struct TEST_RMwR : public RMwR_BASIC {
    // 1000010 w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    constexpr static std::string_view pattern = "1000010";

    TEST_RMwR() : RMwR_BASIC("test") { ; }
    ~TEST_RMwR() = default;
};

#endif // RMWR_DIS