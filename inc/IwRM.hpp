#ifndef IWRM_DIS
#define IWRM_DIS
#include "commands.hpp"

class I2RM_BASIC : public Command_t {
protected:
    const char* name;
    uint8_t offset = 0;
    // XXXXXXs(1)w(1) mod(2)XXXr/m(3) data(8) (if sw == 01)data(8)
    union {
        uint8_t raw[6];
        struct __attribute__((packed)) {
            uint8_t w : 1;
            uint8_t s : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            union {
                uint8_t d[4]; // double
                int8_t s;     // single
            } disp;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    void PrintRM();

public:
    I2RM_BASIC(const char* _name) : Command_t(6), name{_name} { ; }
    void PrintCommand(size_t) override;
    ~I2RM_BASIC() = default;
};

class MOV_I2RM : public I2RM_BASIC {
protected:
    // 1100011w(1) mod(2)000r/m(3) data(8) (if w == 1)data(8)

public:
    MOV_I2RM() : I2RM_BASIC("mov") { ; }
    void PrintCommand(size_t) override;
    ~MOV_I2RM() = default;
};
class OR_I2RM : public I2RM_BASIC {
protected:
    // 1000000w(1) mod(2)001r/m(3) data(8) (if sw == 01)data(8)

public:
    OR_I2RM() : I2RM_BASIC("or") { ; }
    void PrintCommand(size_t) override;
    ~OR_I2RM() = default;
};
class AND_I2RM : public I2RM_BASIC {
protected:
    // 1000000w(1) mod(2)001r/m(3) data(8) (if sw == 01)data(8)

public:
    AND_I2RM() : I2RM_BASIC("and") { ; }
    void PrintCommand(size_t) override;
    ~AND_I2RM() = default;
};


class ADD_I2RM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)000r/m(3) data(8) (if sw == 01)data(8)

public:
    ADD_I2RM() : I2RM_BASIC("add") { ; }

    ~ADD_I2RM() = default;
};
class SSB_I2RM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)011r/m(3) data(8) (if sw == 01)data(8)

public:
    SSB_I2RM() : I2RM_BASIC("sbb") { ; }

    ~SSB_I2RM() = default;
};
class SUB_IfRM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)101r/m(3) data(8) (if sw == 01)data(8)

public:
    SUB_IfRM() : I2RM_BASIC("sub") { ; }

    ~SUB_IfRM() = default;
};
class TEST_IaRM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)101r/m(3) data(8) (if sw == 01)data(8)

public:
    TEST_IaRM() : I2RM_BASIC("test") { ; }
    void PrintCommand(size_t) override;
    ~TEST_IaRM() = default;
};
class CMP_IwRM : public I2RM_BASIC {
protected:
    // 100000d(1)w(1) mod(2)111r/m(3) data(8) (if sw == 01)data(8)

public:
    CMP_IwRM() : I2RM_BASIC("cmp") { ; }
    ~CMP_IwRM() = default;
};

#endif // IWRM_DIS