#ifndef COMMANDS_DIS
#define COMMANDS_DIS

#include "binary.hpp"
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

extern std::map<uint8_t, std::string> regs_8;
extern std::map<uint8_t, std::string> regs_16;
extern std::map<uint8_t, std::string> rm_memory;
extern bool LOG;

class Command_t {
protected:
    uint8_t frame_length;
    const char* name;

    Command_t(uint8_t, const char*);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    virtual uint8_t GetFrameLength() { return frame_length; }
    virtual void Read(uint8_t*);
    virtual void Disassemble(size_t pos);
    void PrintStatus(Binary_t&);
    virtual void Execute(Binary_t&);
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
    void Disassemble(size_t) override;
    void Execute(Binary_t&) override;
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
    void Execute(Binary_t&) override;
    void Disassemble(size_t) override;
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

    void Disassemble(size_t) override;
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

    void Disassemble(size_t) override;
    ~IN_PORT_VAR() = default;
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
    void Execute(Binary_t&) override;
    void Disassemble(size_t) override;
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
    void Execute(Binary_t&) override;
    void Disassemble(size_t) override;

    CALL_DS() : Command_t(size_max, "call") { ; }
    ~CALL_DS() = default;
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
    void Disassemble(size_t) override;
    void Execute(Binary_t&) override;

    RET_wSAI() : Command_t(size_max, "ret") { ; }
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
    void Disassemble(size_t) override;
    ~MOV_MwA() = default;
};

#endif // COMMANDS_DIS