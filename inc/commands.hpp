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
    Command_t(uint8_t);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    virtual uint8_t GetFrameLength() { return frame_length; }
    virtual void Read(uint8_t*);
    virtual void PrintCommand(size_t pos);
    virtual ~Command_t() = default;
};

class MOV_I2R : public Command_t {
public:
    constexpr static size_t size = 3;
    constexpr static std::string_view pattern = "1011";

protected:
    // 1101 w(1) reg(3)  data(8/16)
    union {
        uint8_t raw[size];
        struct {
            uint8_t reg : 3;
            uint8_t w : 1;
            uint8_t : 4;
            uint8_t data[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_I2R() : Command_t(size) { ; }
    void PrintCommand(size_t) override;
    ~MOV_I2R() = default;
};
class INT : public Command_t {
public:
    constexpr static std::string_view pattern = "11001101";

protected:
    // 11001101 type(8)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            uint8_t type;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    INT() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~INT() = default;
};
class IN_PORT : public Command_t {
public:
    constexpr static std::string_view pattern = "1110010";

protected:
    // 1110010w(1) port(8)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t port;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    IN_PORT() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~IN_PORT() = default;
};
class SHL : public Command_t {
public:
    constexpr static std::string_view pattern = "110100XXXX100";

protected:
    // 110100v(1)w(1) mod(2)100r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t v : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    SHL() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~SHL() = default;
};
class SHR : public Command_t {
public:
    constexpr static std::string_view pattern = "110100XXXX101";

protected:
    // 110100 v(1)w(1) mod(2)100r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t v : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    SHR() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~SHR() = default;
};
class SAR : public Command_t {
public:
    constexpr static std::string_view pattern = "110100XXXX111";

protected:
    // 110100 v(1)w(1) mod(2) 111 r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t v : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    SAR() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~SAR() = default;
};
class RCL : public Command_t {
public:
    constexpr static std::string_view pattern = "110100XXXX010";

protected:
    // 110100 v(1)w(1) mod(2) 010 r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t v : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    RCL() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~RCL() = default;
};
class DIV : public Command_t {
public:
    constexpr static std::string_view pattern = "1111011XXX110";

protected:
    // 1111011 w(1) mod(2) 110 r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    DIV() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~DIV() = default;
};
class IN_PORT_VAR : public Command_t {
public:
    constexpr static std::string_view pattern = "1110110";

protected:
    // 1110010w(1)
    union {
        uint8_t raw[1];
        struct {
            uint8_t w : 1;
            uint8_t : 7;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    IN_PORT_VAR() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~IN_PORT_VAR() = default;
};
class PUSH_R : public Command_t {
public:
    constexpr static std::string_view pattern = "01010";

protected:
    // 01010 reg(3)
    union {
        uint8_t raw[1];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    PUSH_R() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~PUSH_R() = default;
};
class REP_MOVS : public Command_t {
public:
    constexpr static std::string_view pattern = "111100101010010";

protected:
    // 1111001 z(1) 1010010 w(1)
    union {
        uint8_t raw[2];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_MOVS() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~REP_MOVS() = default;
};
class REP_STOS : public Command_t {
public:
    constexpr static std::string_view pattern = "111100101010101";

protected:
    // 1111001 z(1) 1010010 w(1)
    union {
        uint8_t raw[2];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_STOS() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~REP_STOS() = default;
};
class CMPS : public Command_t {
public:
    constexpr static std::string_view pattern = "1010011";

protected:
    // 1010011 w(1)
    union {
        uint8_t raw[1];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CMPS() : Command_t(1) { ; }
    void PrintCommand(size_t) override;
    ~CMPS() = default;
};
class REP_SCAS : public Command_t {
public:
    constexpr static std::string_view pattern = "111100101010111";

protected:
    // 1111001 z(1) 1010111 w(1)
    union {
        uint8_t raw[2];
        struct {
            uint8_t z : 1;
            uint8_t : 7;
            uint8_t w : 1;
            uint8_t : 7;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    REP_SCAS() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~REP_SCAS() = default;
};
class DEC_R : public Command_t {
public:
    constexpr static std::string_view pattern = "01001";

protected:
    // 01001 reg(3)
    union {
        uint8_t raw[1];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    DEC_R() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~DEC_R() = default;
};
class POP_R : public Command_t {
public:
    constexpr static std::string_view pattern = "01011";

protected:
    // 01011 reg(3)
    union {
        uint8_t raw[1];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    POP_R() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~POP_R() = default;
};
class INC_R : public Command_t {
public:
    constexpr static std::string_view pattern = "01000";

protected:
    // 01000 reg(3)
    union {
        uint8_t raw[1];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    INC_R() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~INC_R() = default;
};
class CALL_IS : public Command_t {
public:
    constexpr static std::string_view pattern = "11111111XX010";

protected:
    // 11111111 mod(2)010r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CALL_IS() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~CALL_IS() = default;
};
class CALL_DS : public Command_t {
public:
    constexpr static std::string_view pattern = "11101000";

protected:
    // 11101000 disp-low(8) disp-high(8)
    union {
        uint8_t raw[3];
        struct __attribute__((packed)) {
            uint8_t : 8;
            int16_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CALL_DS() : Command_t(3) { ; }

    void PrintCommand(size_t) override;
    ~CALL_DS() = default;
};
class HLT : public Command_t {
public:
    constexpr static std::string_view pattern = "11110100";

protected:
    //
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    HLT() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~HLT() = default;
};
class CBW : public Command_t {
public:
    constexpr static std::string_view pattern = "10011000";

protected:
    // 10011000
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CBW() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~CBW() = default;
};
class CLD : public Command_t {
public:
    constexpr static std::string_view pattern = "11111100";

protected:
    // 11111100
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CLD() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~CLD() = default;
};
class STD : public Command_t {
public:
    constexpr static std::string_view pattern = "11111101";

protected:
    // 11111101
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    STD() : Command_t(1) { ; }
    void PrintCommand(size_t) override;
    ~STD() = default;
};
class CWD : public Command_t {
public:
    constexpr static std::string_view pattern = "10011001";

protected:
    // 10011001
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CWD() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~CWD() = default;
};
class RET : public Command_t {
public:
    constexpr static std::string_view pattern = "11000011";

protected:
    // 11000011
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    RET() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~RET() = default;
};
class SUB_IfA : public Command_t {
public:
    constexpr static std::string_view pattern = "0010110";

protected:
    // 0010110w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[3];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    SUB_IfA() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~SUB_IfA() = default;
};
class CMP_IwA : public Command_t {
public:
    constexpr static std::string_view pattern = "0011110";

protected:
    // 0011110 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[3];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CMP_IwA() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~CMP_IwA() = default;
};
class TEST_IwA : public Command_t {
public:
    constexpr static std::string_view pattern = "1010100";

protected:
    // 1010100 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[3];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    TEST_IwA() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~TEST_IwA() = default;
};
class ADD_IwA : public Command_t {
public:
    constexpr static std::string_view pattern = "0000010";

protected:
    // 0000010 w(1) data(8) (if w == 1)data(8)
    union {
        uint8_t raw[3];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    ADD_IwA() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~ADD_IwA() = default;
};
class XCHG_RwA : public Command_t {
public:
    constexpr static std::string_view pattern = "10010";

protected:
    // 10010 reg(3)
    union {
        uint8_t raw[1];
        struct {
            uint8_t reg : 3;
            uint8_t : 5;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    XCHG_RwA() : Command_t(1) { ; }
    void PrintCommand(size_t) override;
    ~XCHG_RwA() = default;
};
class RET_wSAI : public Command_t {
public:
    constexpr static std::string_view pattern = "11000010";

protected:
    // 11000010 data-low(8) data-high(8)
    union {
        uint8_t raw[3];
        struct __attribute__((packed)) {
            uint8_t : 8;
            uint8_t disp_low;
            uint8_t disp_high;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    RET_wSAI() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~RET_wSAI() = default;
};
class MOV_MwA : public Command_t {
public:
    constexpr static std::string_view pattern = "1010000";

protected:
    // 1010000 w(1) addr_low(8) addr_high(8)
    union {
        uint8_t raw[3];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t addr_low;
            uint8_t addr_high;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_MwA() : Command_t(3) { ; }
    void PrintCommand(size_t) override;
    ~MOV_MwA() = default;
};

#endif // COMMANDS_DIS