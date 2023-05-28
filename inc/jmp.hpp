#ifndef JMP_DIS
#define JMP_DIS

#include "commands.hpp"

class JMP_BASIC : public Command_t {
protected:
    // XXXXXXXX disp
    const char* name;
    union {
        uint8_t raw[2];
        struct {
            uint8_t : 8;
            int8_t disp;
        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }

    JMP_BASIC(const char* _n, size_t _s = 2) : Command_t(_s), name{_n} { ; }

public:
    void PrintCommand(size_t) override;
    ~JMP_BASIC() = default;
};
class JNBE : public JMP_BASIC {
protected:
    // 01110111 disp(8)
public:
    JNBE() : JMP_BASIC("jnbe") { ; }
    ~JNBE() = default;
};
class JBE : public JMP_BASIC {
protected:
    // 01110110 disp(8)
public:
    JBE() : JMP_BASIC("jbe") { ; }
    ~JBE() = default;
};
class JL : public JMP_BASIC {
protected:
    // 01111100 disp(8)
public:
    JL() : JMP_BASIC("jl") { ; }
    ~JL() = default;
};
class JLE : public JMP_BASIC {
protected:
    // 01111110 disp(8)
public:
    JLE() : JMP_BASIC("jle") { ; }
    ~JLE() = default;
};
class JB : public JMP_BASIC {
protected:
    // 01110010 disp(8)
public:
    JB() : JMP_BASIC("jb") { ; }
    ~JB() = default;
};
class JNB : public JMP_BASIC {
protected:
    // 01110011 disp(8)
public:
    JNB() : JMP_BASIC("jnb") { ; }
    ~JNB() = default;
};
class JNL : public JMP_BASIC {
protected:
    // 01111101 disp(8)
public:
    JNL() : JMP_BASIC("jnl") { ; }
    ~JNL() = default;
};
class JNE : public JMP_BASIC {
protected:
    // 01110101 disp(8)
public:
    JNE() : JMP_BASIC("jne") { ; }
    ~JNE() = default;
};
class JE : public JMP_BASIC {
protected:
    // 01110100 disp(8)
public:
    JE() : JMP_BASIC("je") { ; }
    ~JE() = default;
};
class JMP_DSS : public JMP_BASIC {
protected:
    // 11101011 disp(8)
public:
    JMP_DSS() : JMP_BASIC("jmp short") { ; }
    ~JMP_DSS() = default;
};
class JMP_DS : public JMP_BASIC {
protected:
    // 11101001 disp(8)
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
    JMP_DS() : JMP_BASIC("jmp", 3) { ; }
    void PrintCommand(size_t) override;
    ~JMP_DS() = default;
};
class JNLE : public JMP_BASIC {
protected:
    // 01111111 disp(8)
public:
    JNLE() : JMP_BASIC("jnle") { ; }
    ~JNLE() = default;
};
class JS : public JMP_BASIC {
protected:
    // 01111111 disp(8)
public:
    JS() : JMP_BASIC("js") { ; }
    ~JS() = default;
};
class LOOP : public JMP_BASIC {
protected:
    // 11100010 disp(8)
public:
    LOOP() : JMP_BASIC("loop") { ; }
    ~LOOP() = default;
};

class JMP_IS : public Command_t {
protected:
    // 11111111 mod(2)101r/m(3)
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
    JMP_IS() : Command_t(2) { ; }
    void PrintCommand(size_t) override;
    ~JMP_IS() = default;
};

#endif // JMP_DIS