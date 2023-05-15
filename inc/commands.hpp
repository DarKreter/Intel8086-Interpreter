#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>

extern std::map<uint8_t, std::string> regs_8;
extern std::map<uint8_t, std::string> regs_16;

class Command_t {
protected:
    const uint8_t frame_length;
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