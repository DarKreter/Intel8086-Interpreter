#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include "message.hpp"
#include <unistd.h>
// #include <bitset>

using namespace std;

void Command_t::Execute(Binary_t&, bool) { ; }

void MOV_I2R::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << ", ";

    printf("%02x", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02x", frame.decoded.data[0]);
}

void CALL_DS::Execute(Binary_t& binary, bool)
{
    uint16_t pos = binary.textPos + 3;
    binary.stack[--binary.sp] = pos >> 8;
    binary.stack[--binary.sp] = pos;
    // printf("!CAD:%x!", pos);

    binary.textPos += (int)(frame.decoded.disp);
    binary.text += (int)(frame.decoded.disp);
}

void CALL_IS::Execute(Binary_t& binary, bool)
{
    uint16_t pos = binary.textPos + 2;
    uint16_t disp = binary.GetReg(1, frame.decoded.rm);
    binary.stack[--binary.sp] = pos >> 8;
    binary.stack[--binary.sp] = pos;

    uint8_t* begin = binary.text - binary.textPos;
    binary.textPos = (int)(disp - 2);
    binary.text = begin + binary.textPos;
}

void MOV_I2R::Execute(Binary_t& binary, bool)
{
    auto& reg = binary.GetReg(frame.decoded.w, frame.decoded.reg);

    if(frame.decoded.w == 0)
        reg = frame.decoded.data[0];
    else
        reg = frame.decoded.data[0] + (frame.decoded.data[1] << 8);
}
void RET_wSAI::Execute(Binary_t& binary, bool)
{
    uint16_t pos;
    uint16_t disp = frame.decoded.disp_low;
    disp += (frame.decoded.disp_high << 8);

    pos = binary.stack[binary.sp++];
    pos += binary.stack[binary.sp++] << 8;
    pos -= 3;
    binary.sp += disp;

    // printf("!%x!", pos);
    binary.text += pos - binary.textPos;
    binary.textPos = pos;
}

void RET_wSAI::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    printf("%02x%02x", frame.decoded.disp_high, frame.decoded.disp_low);
}
void IN_PORT::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("%02x", frame.decoded.port);
}
void IN_PORT_VAR::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("dx");
}
void CALL_DS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    printf("%04lx", frame.decoded.disp + pos + 3);
}
void CALL_IS::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    cout << regs_16[frame.decoded.rm];
}

void INT::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);

    std::cout << std::hex << (int)frame.raw[1];
}
void INT::Execute(Binary_t& binary, bool log)
{
    message* mess = (message*)&binary.stack[binary.bx];
    uint16_t addr, fd, request, length;
    int ret;
    if(log)
        printf("\n");
    switch(mess->m_type) {
    case 1: // exit
        if(log)
            printf("<exit(%d)>\n", mess->m_m1.m1i1);
        exit(mess->m_m1.m1i1);
        break;
    case 4: // write
        fd = mess->m_m1.m1i1;
        addr = mess->m_m1.m1p1;
        length = mess->m_m1.m1i2;
        if(log) {
            printf("<write(%d, 0x%04x, %d)", fd, addr, length);
            fflush(stdout);
        }
        ret = write(fd, &binary.stack[addr], length);
        // (ret == -1) ? (mess->m_type = -errno) : (mess->m_type = ret);
        mess->m_type = ret;
        if(log)
            printf(" => %d>", ret);

        binary.ax = 0;

        break;
    case 17: // brk
        addr = mess->m_m1.m1p1;
        binary.ax = 0;

        if(log)
            printf("<brk(0x%04x) => ", addr);

        if(addr < binary.dataSegmentSize ||
           (addr >= ((binary.sp & ~0x3ff) - 0x400))) {
            errno = ENOMEM;
            if(log)
                printf("ENOMEM>");

            mess->m_type = -errno;
        }
        else {
            if(log)
                printf("0>");

            mess->m_type = 0;
            mess->m_m1.m1p5 = addr;
        }

        break;
    case 54:
        fd = mess->m_m1.m1i1;
        request = mess->m_m1.m1i3;
        addr = mess->m_m1.m1p5;

        if(log)
            printf("<ioctl(%d, 0x%04x, 0x%04x)>", fd, request, addr);
        errno = EINVAL;
        mess->m_type = -errno;

        binary.ax = 0;

        break;
    }
}
void INT::PrintStatus(Binary_t& bin) { Command_t::PrintStatus(bin); }

void MOV_MwA::Disassemble(size_t pos)
{
    Command_t::Disassemble(pos);
    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    printf("[%02x%02x]", frame.decoded.addr_high, frame.decoded.addr_low);
}

Command_t::Command_t(uint8_t _frame_length, const char* _name)
    : frame_length{_frame_length}, name{_name}
{
}

void Command_t::PrintStatus(Binary_t& binary)
{
    binary.PrintStatus();
    Disassemble(binary.textPos);
}

void Command_t::Disassemble(size_t pos)
{
    printf("%04lx: ", pos);
    for(uint8_t i = 0; i < frame_length; i++)
        printf("%02x", GetFramePart(i));
    printf("\t\t%s ", name);
}
void Command_t::Read(uint8_t* tab)
{
    for(int i = 0; i < frame_length; i++)
        GetFramePart(i) = *(tab++);
}
std::map<uint8_t, std::string> regs_8 = {{0, "al"}, {1, "cl"}, {2, "dl"},
                                         {3, "bl"}, {4, "ah"}, {5, "ch"},
                                         {6, "dh"}, {7, "bh"}};
std::map<uint8_t, std::string> regs_16 = {{0, "ax"}, {1, "cx"}, {2, "dx"},
                                          {3, "bx"}, {4, "sp"}, {5, "bp"},
                                          {6, "si"}, {7, "di"}};
std::map<uint8_t, std::string> rm_memory = {
    {0, "bx+si"}, {1, "bx+di"}, {2, "bp+si"}, {3, "bp+di"},
    {4, "si"},    {5, "di"},    {6, "bp"},    {7, "bx"}};

#endif // COMMANDS_DISASSEMBLY