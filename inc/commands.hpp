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
    Command_t(uint8_t fl);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    uint8_t GetFrameLength() { return frame_length; }
    virtual void Read(uint8_t*);
    virtual void PrintCommand(size_t pos);
    virtual ~Command_t() = default;
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
class IN_PORT : public Command_t {
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
class IN_PORT_VAR : public Command_t {
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
class NEG : public Command_t {
protected:
    // 1111011w(1) mod(2)011r/m(3) disp(0/8/16)
    union {
        uint8_t raw[4];
        struct {
            uint8_t : 8;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            uint8_t disp[2];
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

public:
    NEG() : Command_t(4) { ; }

    void PrintCommand(size_t) override;
    ~NEG() = default;
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
class POP_R : public Command_t {
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
class RET : public Command_t {
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

#endif // COMMANDS_DIS