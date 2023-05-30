#ifndef COMMANDS_DIS
#define COMMANDS_DIS

#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

extern std::map<uint8_t, std::string> regs_8;
extern std::map<uint8_t, std::string> regs_16;
extern std::map<uint8_t, std::string> rm_memory;

class Command_t {
protected:
    uint8_t frame_length;
    const char* name;

    Command_t(uint8_t, const char*);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    virtual uint8_t GetFrameLength() { return frame_length; }
    virtual void Read(uint8_t*);
    virtual void PrintCommand(size_t pos);
    virtual ~Command_t() = default;
};

struct MOV_I2R : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "1011";

protected:
    // 1101 w(1) reg(3)  data(8/16)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t w : 1;
            uint8_t : 4;
            uint8_t data[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_I2R() : Command_t(size_max, "mov") { ; }
    void PrintCommand(size_t) override;
    ~MOV_I2R() = default;
};
struct INT : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "11001101";

protected:
    // 11001101 type(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t : 8;
            uint8_t type;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    INT() : Command_t(size_max, "int") { ; }

    void PrintCommand(size_t) override;
    ~INT() = default;
};
struct IN_PORT : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "1110010";

protected:
    // 1110010w(1) port(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t port;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    IN_PORT() : Command_t(size_max, "in") { ; }

    void PrintCommand(size_t) override;
    ~IN_PORT() = default;
};
struct IN_PORT_VAR : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "1110110";

protected:
    // 1110010w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    IN_PORT_VAR() : Command_t(size_max, "in") { ; }

    void PrintCommand(size_t) override;
    ~IN_PORT_VAR() = default;
};
struct PUSH_R : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "01010";

protected:
    // 01010 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    PUSH_R() : Command_t(size_max, "push") { ; }

    void PrintCommand(size_t) override;
    ~PUSH_R() = default;
};
struct REP_MOVS : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "111100101010010";

protected:
    // 1111001 z(1) 1010010 w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_MOVS() : Command_t(size_max, "rep movs") { ; }
    void PrintCommand(size_t) override;
    ~REP_MOVS() = default;
};
struct REP_STOS : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "111100101010101";

protected:
    // 1111001 z(1) 1010010 w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_STOS() : Command_t(size_max, "rep stos") { ; }
    void PrintCommand(size_t) override;
    ~REP_STOS() = default;
};
struct CMPS : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "1010011";

protected:
    // 1010011 w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CMPS() : Command_t(size_max, "cmps") { ; }
    void PrintCommand(size_t) override;
    ~CMPS() = default;
};
struct REP_SCAS : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "111100101010111";

protected:
    // 1111001 z(1) 1010111 w(1)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_SCAS() : Command_t(size_max, "rep scas") { ; }
    void PrintCommand(size_t) override;
    ~REP_SCAS() = default;
};
struct DEC_R : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "01001";

protected:
    // 01001 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    DEC_R() : Command_t(size_max, "dec") { ; }

    void PrintCommand(size_t) override;
    ~DEC_R() = default;
};
struct POP_R : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "01011";

protected:
    // 01011 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    POP_R() : Command_t(size_max, "pop") { ; }

    void PrintCommand(size_t) override;
    ~POP_R() = default;
};
struct INC_R : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "01000";

protected:
    // 01000 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    INC_R() : Command_t(size_max, "inc") { ; }

    void PrintCommand(size_t) override;
    ~INC_R() = default;
};
struct CALL_IS : public Command_t {
    constexpr static size_t size_max = 2;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "11111111XX010";

protected:
    // 11111111 mod(2)010r/m(3)
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
    CALL_IS() : Command_t(size_max, "call") { ; }

    void PrintCommand(size_t) override;
    ~CALL_IS() = default;
};
struct CALL_DS : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 3;
    constexpr static std::string_view pattern = "11101000";

protected:
    // 11101000 disp-low(8) disp-high(8)
    union {
        uint8_t raw[size_max];
        struct __attribute__((packed)) {
            uint8_t : 8;
            int16_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CALL_DS() : Command_t(size_max, "call") { ; }

    void PrintCommand(size_t) override;
    ~CALL_DS() = default;
};
struct HLT : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "11110100";

protected:
    //
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    HLT() : Command_t(size_max, "hlt") { ; }

    void PrintCommand(size_t) override;
    ~HLT() = default;
};
struct CBW : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "10011000";

protected:
    // 10011000
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CBW() : Command_t(size_max, "cbw") { ; }

    void PrintCommand(size_t) override;
    ~CBW() = default;
};
struct CLD : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "11111100";

protected:
    // 11111100
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CLD() : Command_t(size_max, "cld") { ; }

    void PrintCommand(size_t) override;
    ~CLD() = default;
};
struct STD : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "11111101";

protected:
    // 11111101
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    STD() : Command_t(size_max, "std") { ; }
    void PrintCommand(size_t) override;
    ~STD() = default;
};
struct CWD : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "10011001";

protected:
    // 10011001
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CWD() : Command_t(size_max, "cwd") { ; }

    void PrintCommand(size_t) override;
    ~CWD() = default;
};
struct RET : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "11000011";

protected:
    // 11000011
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    RET() : Command_t(size_max, "ret") { ; }

    void PrintCommand(size_t) override;
    ~RET() = default;
};
struct SUB_IfA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "0010110";

protected:
    // 0010110w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    SUB_IfA() : Command_t(size_max, "sub") { ; }
    void PrintCommand(size_t) override;
    ~SUB_IfA() = default;
};
struct CMP_IwA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "0011110";

protected:
    // 0011110 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CMP_IwA() : Command_t(size_max, "cmp") { ; }
    void PrintCommand(size_t) override;
    ~CMP_IwA() = default;
};
struct TEST_IwA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "1010100";

protected:
    // 1010100 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    TEST_IwA() : Command_t(size_max, "test") { ; }
    void PrintCommand(size_t) override;
    ~TEST_IwA() = default;
};
struct ADD_IwA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 2;
    constexpr static std::string_view pattern = "0000010";

protected:
    // 0000010 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    ADD_IwA() : Command_t(size_max, "add") { ; }
    void PrintCommand(size_t) override;
    ~ADD_IwA() = default;
};
struct XCHG_RwA : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;
    constexpr static std::string_view pattern = "10010";

protected:
    // 10010 reg(3)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    XCHG_RwA() : Command_t(size_max, "xchg") { ; }
    void PrintCommand(size_t) override;
    ~XCHG_RwA() = default;
};
struct RET_wSAI : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 3;
    constexpr static std::string_view pattern = "11000010";

protected:
    // 11000010 data-low(8) data-high(8)
    union {
        uint8_t raw[size_max];
        struct __attribute__((packed)) {
            uint8_t : 8;
            uint8_t disp_low;
            uint8_t disp_high;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    RET_wSAI() : Command_t(size_max, "ret") { ; }
    void PrintCommand(size_t) override;
    ~RET_wSAI() = default;
};
struct MOV_MwA : public Command_t {
    constexpr static size_t size_max = 3;
    constexpr static size_t size_min = 3;
    constexpr static std::string_view pattern = "1010000";

protected:
    // 1010000 w(1) addr_low(8) addr_high(8)
    union {
        uint8_t raw[size_max];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t addr_low;
            uint8_t addr_high;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_MwA() : Command_t(size_max, "mov") { ; }
    void PrintCommand(size_t) override;
    ~MOV_MwA() = default;
};

#endif // COMMANDS_DIS