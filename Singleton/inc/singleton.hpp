#ifndef SINGLETON_DIS
#define SINGLETON_DIS

#include "commands.hpp"

struct Singleton : public Command_t {
    constexpr static size_t size_max = 1;
    constexpr static size_t size_min = 1;

protected:
    // XXXXXXXX
    union {
        uint8_t raw[size_max];
    } frame;

    uint8_t GetFramePart(uint8_t i) const override { return frame.raw[i]; }
    void SetFramePart(uint8_t i, uint8_t val) override { frame.raw[i] = val; }
    void PrintBase(size_t pos);

public:
    Singleton(const char* _n, size_t _s = size_max) : Command_t(_s, _n) { ; }
    ~Singleton() = default;
};
struct HLT : public Singleton {
    // 11110100
    constexpr static std::string_view pattern = "11110100";

    HLT() : Singleton("hlt") { ; }
    ~HLT() = default;
};
struct CBW : public Singleton {
    // 10011000
    constexpr static std::string_view pattern = "10011000";

    void Execute(Binary_t&) override;
    CBW() : Singleton("cbw") { ; }
    ~CBW() = default;
};
struct CLD : public Singleton {
    // 11111100
    constexpr static std::string_view pattern = "11111100";

    void Execute(Binary_t&) override { ; };
    CLD() : Singleton("cld") { ; }
    ~CLD() = default;
};
struct STD : public Singleton {
    // 11111101
    constexpr static std::string_view pattern = "11111101";

    STD() : Singleton("std") { ; }
    ~STD() = default;
};
struct CWD : public Singleton {
    // 10011001
    constexpr static std::string_view pattern = "10011001";

    void Execute(Binary_t&) override;
    CWD() : Singleton("cwd") { ; }
    ~CWD() = default;
};
struct RET : public Singleton {
    // 11000011
    constexpr static std::string_view pattern = "11000011";

    void Execute(Binary_t&) override;
    RET() : Singleton("ret") { ; }
    ~RET() = default;
};

#endif // SINGLETON_DIS