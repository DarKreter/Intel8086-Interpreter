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
    Command_t(uint8_t fl);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    uint8_t GetFrameLength() { return frame_length; }
    virtual void Read(uint8_t*);
    virtual void PrintCommand(size_t pos);
    virtual ~Command_t() = default;
};
class INT : public Command_t {
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
class ADD_RMwR : public Command_t {
protected:
    // 000000d(1)w(1) mod(2)reg(3)r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t d : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    ADD_RMwR() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~ADD_RMwR() = default;
};
class ADD_I2RM : public Command_t {
protected:
    // 100000d(1)w(1) mod(2)000r/m(3) data(8) (if sw == 01)data(8)
    union {
        uint8_t raw[4];
        struct {
            uint8_t w : 1;
            uint8_t s : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            uint8_t data[2];

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    ADD_I2RM() : Command_t(4) { ; }

    void PrintCommand(size_t) override;
    ~ADD_I2RM() = default;
};
class SHL : public Command_t {
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
class XOR_RM2R : public Command_t {
protected:
    // 001100d(1)w(1) mod(2)reg(3)r/m(3)
    union {
        uint8_t raw[2];
        struct {
            uint8_t w : 1;
            uint8_t d : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    XOR_RM2R() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~XOR_RM2R() = default;
};
class MOV_I2R : public Command_t {
protected:
    // 1101 w(1) reg(3)  data(8/16)
    union {
        uint8_t raw[3];
        struct {
            uint8_t reg : 3;
            uint8_t w : 1;
            uint8_t : 4;
            uint8_t data[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_I2R() : Command_t(3) { ; }

    void PrintCommand(size_t) override;
    ~MOV_I2R() = default;
};
class MOV_RM2R : public Command_t {
protected:
    // 100010 d(1)w(1) mod(2)reg(3)r/m(3)
    union {
        uint8_t raw[4];
        struct {
            uint8_t w : 1;
            uint8_t d : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
            uint8_t disp[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    MOV_RM2R() : Command_t(4) { ; }

    void PrintCommand(size_t) override;
    ~MOV_RM2R() = default;
};
class LEA : public Command_t {
protected:
    // 10001101 mod(2)reg(3)r/m(3)
    union {
        uint8_t raw[3];
        struct {
            uint8_t : 8;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    LEA() : Command_t(3) { ; }

    void PrintCommand(size_t) override;
    ~LEA() = default;
};
class CMP_IwRM : public Command_t {
protected:
    // 100000 d(1)w(1) mod(2)111r/m(3) disp(0/8/16) data(8/16)
    union {
        uint8_t raw[6];
        struct {
            uint8_t w : 1;
            uint8_t s : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            int8_t data[4];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    CMP_IwRM() : Command_t(6) { ; }

    void PrintCommand(size_t) override;
    ~CMP_IwRM() = default;
};
class PUSH_R : public Command_t {
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
class DEC_R : public Command_t {
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
class JNB : public Command_t {
protected:
    // 100000 d(1)w(1) mod(2)111r/m(3) data(8/16)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    JNB() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~JNB() = default;
};
class JL : public Command_t {
protected:
    // 01111100 disp(8)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    JL() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~JL() = default;
};
class JNE : public Command_t {
protected:
    // 01110101 disp(8)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    JNE() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~JNE() = default;
};
class JMP_DS : public Command_t {
protected:
    // 11101001 disp(16)
    union {
        uint8_t raw[3];
        struct {
            uint8_t : 8;
            uint8_t disp_low;
            uint8_t disp_high;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    JMP_DS() : Command_t(3) { ; }

    void PrintCommand(size_t) override;
    ~JMP_DS() = default;
};
class JMP_DSS : public Command_t {
protected:
    // 11101011 disp(8)
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    JMP_DSS() : Command_t(2) { ; }

    void PrintCommand(size_t) override;
    ~JMP_DSS() = default;
};
class TEST_IaRM : public Command_t {
protected:
    // 100000 d(1)w(1) mod(2)111r/m(3) data(8/16)
    union {
        uint8_t raw[4];
        struct {
            uint8_t w : 1;
            uint8_t : 7;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            int8_t data[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    TEST_IaRM() : Command_t(4) { ; }

    void PrintCommand(size_t) override;
    ~TEST_IaRM() = default;
};
class CALL_IS : public Command_t {
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
protected:
    // 11101000 disp-low(8) disp-high(8)
    union {
        uint8_t raw[1];
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    HLT() : Command_t(1) { ; }

    void PrintCommand(size_t) override;
    ~HLT() = default;
};
