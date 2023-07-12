#ifndef IWRM_DIS
#define IWRM_DIS
#include "commands.hpp"

struct I2RM_BASIC : public Command_t {
    constexpr static size_t size_max = 6;
    constexpr static size_t size_min = 3;

protected:
    uint8_t offset = 0;
    // XXXXXXs(1)w(1) mod(2)XXXr/m(3) data(8) (if sw == 01)data(8)
    union {
        uint8_t raw[size_max];
        struct __attribute__((packed)) {
            uint8_t w : 1;
            uint8_t s : 1;
            uint8_t : 6;
            uint8_t rm : 3;
            uint8_t : 3;
            uint8_t mod : 2;
            union {
                uint8_t d[size_max - 2]; // double
                int8_t s;                // single
            } disp;

        } decoded;
    } frame;

    uint8_t& GetFramePart(uint8_t i) override { return frame.raw[i]; }
    void PrintRM();
    void SetRM(Binary_t& binary, uint16_t val);
    uint16_t GetRM(Binary_t&, bool);
    uint16_t GetRM_addr(Binary_t&);

public:
    I2RM_BASIC(const char* _name) : Command_t(size_max, _name) { ; }
    void Disassemble(size_t) override;
    ~I2RM_BASIC() = default;
};

struct I2RM_BASIC_s : public I2RM_BASIC {
    // XXXXXXs(1)w(1) mod(2)XXXr/m(3) data(8) (if sw == 01)data(8)
    void Disassemble(size_t) override;
    ~I2RM_BASIC_s() = default;

protected:
    I2RM_BASIC_s(const char* _name) : I2RM_BASIC(_name) { ; }
};
struct MOV_I2RM : public I2RM_BASIC {
    // 1100011 w(1) mod(2) 000 r/m(3) data(8) (if w == 1)data(8)
    constexpr static std::string_view pattern = "1100011XXX000";

    MOV_I2RM() : I2RM_BASIC("mov") { ; }
    ~MOV_I2RM() = default;
};
struct OR_I2RM : public I2RM_BASIC {
    // 1000000 w(1) mod(2) 001 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "1000000XXX001";

    OR_I2RM() : I2RM_BASIC("or") { ; }
    ~OR_I2RM() = default;
};
struct AND_I2RM : public I2RM_BASIC {
    // 1000000 w(1) mod(2) 001 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "1000000XXX100";

    AND_I2RM() : I2RM_BASIC("and") { ; }
    ~AND_I2RM() = default;
};
struct TEST_IaRM : public I2RM_BASIC {
    // 100000 s(1)w(1) mod(2) 101 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "1111011XXX000";

    void Execute(Binary_t&, bool = false) override;
    TEST_IaRM() : I2RM_BASIC("test") { ; }
    ~TEST_IaRM() = default;
};
struct ADD_I2RM : public I2RM_BASIC_s {
    // 100000 s(1)w(1) mod(2) 000 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "100000XXXX000";

    void Execute(Binary_t&, bool = false) override;
    ADD_I2RM() : I2RM_BASIC_s("add") { ; }
    ~ADD_I2RM() = default;
};
struct SSB_I2RM : public I2RM_BASIC_s {
    // 100000 s(1)w(1) mod(2) 011 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "100000XXXX011";

    SSB_I2RM() : I2RM_BASIC_s("sbb") { ; }
    ~SSB_I2RM() = default;
};
struct SUB_IfRM : public I2RM_BASIC_s {
    // 100000 s(1)w(1) mod(2) 101 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "100000XXXX101";

    void Execute(Binary_t&, bool = false) override;
    SUB_IfRM() : I2RM_BASIC_s("sub") { ; }
    ~SUB_IfRM() = default;
};
struct CMP_IwRM : public I2RM_BASIC_s {
    // 100000 s(1)w(1) mod(2) 111 r/m(3) data(8) (if sw == 01)data(8)
    constexpr static std::string_view pattern = "100000XXXX111";

    void Execute(Binary_t&, bool = false) override;
    CMP_IwRM() : I2RM_BASIC_s("cmp") { ; }
    ~CMP_IwRM() = default;
};

#endif // IWRM_DIS