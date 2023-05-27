#ifndef IWRM_DIS
#define IWRM_DIS
#include "commands.hpp"

class I2RM_BASIC : public Command_t {
protected:
    const char* name;
    // XXXXXXd(1)w(1) mod(2)XXXr/m(3) data(8) (if sw == 01)data(8)
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
    I2RM_BASIC(const char* _name) : Command_t(4), name{_name} { ; }
    void PrintCommand(size_t) override;
    ~I2RM_BASIC() = default;
};

class ADD_I2RM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)000r/m(3) data(8) (if sw == 01)data(8)

public:
    ADD_I2RM() : I2RM_BASIC("add") { ; }

    ~ADD_I2RM() = default;
};
class SUB_IfRM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)101r/m(3) data(8) (if sw == 01)data(8)

public:
    SUB_IfRM() : I2RM_BASIC("sub") { ; }

    ~SUB_IfRM() = default;
};


#endif // IWRM_DIS