#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class Command_t {
protected:
    const uint8_t frame_length;
    Command_t(uint8_t fl);

    virtual uint8_t& GetFramePart(uint8_t) = 0;

public:
    virtual void Read(uint8_t*);
    virtual void PrintCommand() = 0;
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

    void PrintCommand() override;
    ~MOV_I2R() = default;
};
