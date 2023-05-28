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
class INC_R : public Command_t {
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
class CBW : public Command_t {
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
class CWD : public Command_t {
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

#endif // COMMANDS_DIS