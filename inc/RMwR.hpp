#ifndef RMWR_DIS
#define RMWR_DIS

#include "commands.hpp"

class RMwR_BASIC : public Command_t {
protected:
    constexpr static size_t size = 4;
    const char* name;
    // XXXXXXd(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
    union {
        uint8_t raw[size];
        struct __attribute__((packed)) {
            uint8_t w : 1;
            uint8_t d : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t reg : 3;
            uint8_t mod : 2;
            union {
                uint8_t d[2]; // double
                int8_t s;     // single
            } disp;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    void PrintRM();

public:
    RMwR_BASIC(const char* _name) : Command_t(size), name{_name} { ; }

    void PrintCommand(size_t) override;
    ~RMwR_BASIC() = default;
};

class NEG : public RMwR_BASIC {
protected:
    // 1111011w(1) mod(2)011r/m(3) disp(0/8/16)
public:
    NEG() : RMwR_BASIC("neg") { ; }
    void PrintCommand(size_t) override;
    ~NEG() = default;
};
class ADD_RMwR : public RMwR_BASIC {
protected:
    // 000000d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
public:
    ADD_RMwR() : RMwR_BASIC("add") { ; }

    ~ADD_RMwR() = default;
};
class SUB_RM2R : public RMwR_BASIC {
protected:
    // 001010 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
public:
    SUB_RM2R() : RMwR_BASIC("sub") { ; }

    ~SUB_RM2R() = default;
};
class CMP_RMaR : public RMwR_BASIC {
protected:
    // 001110d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)
public:
    CMP_RMaR() : RMwR_BASIC("cmp") { ; }

    ~CMP_RMaR() = default;
};
class XOR_RM2R : public RMwR_BASIC {
protected:
    // 001100d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    XOR_RM2R() : RMwR_BASIC("xor") { ; }

    ~XOR_RM2R() = default;
};
class SBB_RMaR : public RMwR_BASIC {
protected:
    // 000000d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    SBB_RMaR() : RMwR_BASIC("sbb") { ; }

    ~SBB_RMaR() = default;
};
class OR_RMaR : public RMwR_BASIC {
protected:
    // 000010d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    OR_RMaR() : RMwR_BASIC("or") { ; }

    ~OR_RMaR() = default;
};
class MOV_RM2R : public RMwR_BASIC {
protected:
    // 100010 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    MOV_RM2R() : RMwR_BASIC("mov") { ; }

    ~MOV_RM2R() = default;
};
class AND_RMaR : public RMwR_BASIC {
protected:
    // 100010 d(1)w(1) mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    AND_RMaR() : RMwR_BASIC("and") { ; }

    ~AND_RMaR() = default;
};
class LEA : public RMwR_BASIC {
protected:
    // 10001101 mod(2)reg(3)r/m(3) disp(0/8/16)

public:
    LEA() : RMwR_BASIC("lea") { ; }
    void PrintCommand(size_t) override;
    ~LEA() = default;
};
class PUSH_RM : public RMwR_BASIC {
protected:
    // 11111111 mod(2)110r/m(3) disp(0/8/16)
public:
    PUSH_RM() : RMwR_BASIC("push") { ; }
    void PrintCommand(size_t) override;
    ~PUSH_RM() = default;
};
class DEC_RM : public RMwR_BASIC {
protected:
    // 11111111 mod(2)001r/m(3) disp(0/8/16)
public:
    DEC_RM() : RMwR_BASIC("dec") { ; }
    void PrintCommand(size_t) override;
    ~DEC_RM() = default;
};
class INC_RM : public RMwR_BASIC {
protected:
    // 11111111 mod(2)000r/m(3) disp(0/8/16)
public:
    INC_RM() : RMwR_BASIC("inc") { ; }
    void PrintCommand(size_t) override;
    ~INC_RM() = default;
};
class MUL : public RMwR_BASIC {
protected:
    // 1111011w(1) mod(2)100r/m(3) disp(0/8/16)
public:
    MUL() : RMwR_BASIC("mul") { ; }
    void PrintCommand(size_t) override;
    ~MUL() = default;
};

#endif // RMWR_DIS