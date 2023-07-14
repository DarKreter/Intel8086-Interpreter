#ifndef JMP_DIS
#define JMP_DIS

#include "commands.hpp"

struct JMP_BASIC : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;

protected:
    // XXXXXXXX disp
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    JMP_BASIC(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }

public:
    void Disassemble(size_t) override;
    void Execute(Binary_t&) override;

    ~JMP_BASIC() = default;
};

struct JNBE : public JMP_BASIC {
    // 01110111 disp(8)
    constexpr static std::string_view pattern = "01110111";

    void Execute(Binary_t&) override;
    JNBE() : JMP_BASIC("jnbe") { ; }
    ~JNBE() = default;
};
struct JBE : public JMP_BASIC {
    // 01110110 disp(8)
    constexpr static std::string_view pattern = "01110110";

    void Execute(Binary_t&) override;
    JBE() : JMP_BASIC("jbe") { ; }
    ~JBE() = default;
};
struct JL : public JMP_BASIC {
    // 01111100 disp(8)
    constexpr static std::string_view pattern = "01111100";

    void Execute(Binary_t&) override;
    JL() : JMP_BASIC("jl") { ; }
    ~JL() = default;
};
struct JLE : public JMP_BASIC {
    // 01111110 disp(8)
    constexpr static std::string_view pattern = "01111110";

    void Execute(Binary_t&) override;
    JLE() : JMP_BASIC("jle") { ; }
    ~JLE() = default;
};
struct JB : public JMP_BASIC {
    // 01110010 disp(8)
    constexpr static std::string_view pattern = "01110010";

    void Execute(Binary_t&) override;
    JB() : JMP_BASIC("jb") { ; }
    ~JB() = default;
};
struct JNB : public JMP_BASIC {
    // 01110011 disp(8)
    constexpr static std::string_view pattern = "01110011";

    void Execute(Binary_t&) override;
    JNB() : JMP_BASIC("jnb") { ; }
    ~JNB() = default;
};
struct JNL : public JMP_BASIC {
    // 01111101 disp(8)
    constexpr static std::string_view pattern = "01111101";

    void Execute(Binary_t&) override;
    JNL() : JMP_BASIC("jnl") { ; }
    ~JNL() = default;
};
struct JNE : public JMP_BASIC {
    // 01110101 disp(8)
    constexpr static std::string_view pattern = "01110101";

    void Execute(Binary_t&) override;
    JNE() : JMP_BASIC("jne") { ; }
    ~JNE() = default;
};
struct JE : public JMP_BASIC {
    // 01110100 disp(8)
    constexpr static std::string_view pattern = "01110100";

    void Execute(Binary_t&) override;
    JE() : JMP_BASIC("je") { ; }
    ~JE() = default;
};
struct JMP_DSS : public JMP_BASIC {
    // 11101011 disp(8)
    constexpr static std::string_view pattern = "11101011";

    // void Execute(Binary_t& ) override;
    JMP_DSS() : JMP_BASIC("jmp short") { ; }
    ~JMP_DSS() = default;
};
struct JMP_DS : public JMP_BASIC {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 3;
    constexpr static std::string_view pattern = "11101001";

protected:
    // 11101001 disp(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t : 8;
            uint8_t disp_low;
            uint8_t disp_high;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    void Disassemble(size_t) override;
    void Execute(Binary_t&);
    JMP_DS() : JMP_BASIC("jmp", size_max) { ; }
    ~JMP_DS() = default;
};
struct JNLE : public JMP_BASIC {
    // 01111111 disp(8)
    constexpr static std::string_view pattern = "01111111";

    void Execute(Binary_t&) override;
    JNLE() : JMP_BASIC("jnle") { ; }
    ~JNLE() = default;
};
struct JS : public JMP_BASIC {
    // 01111111 disp(8)
    constexpr static std::string_view pattern = "01111000";

    JS() : JMP_BASIC("js") { ; }
    ~JS() = default;
};
struct LOOP : public JMP_BASIC {
    // 11100010 disp(8)
    constexpr static std::string_view pattern = "11100010";

    LOOP() : JMP_BASIC("loop") { ; }
    ~LOOP() = default;
};

struct JMP_IS : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "11111111XX100";

protected:
    // 11111111 mod(2)101r/m(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t : 8;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    void Disassemble(size_t) override;
    void Execute(Binary_t&) override;

    JMP_IS() : Command_t(size_max, "jmp") { ; }
    ~JMP_IS() = default;
};

#endif // JMP_DIS