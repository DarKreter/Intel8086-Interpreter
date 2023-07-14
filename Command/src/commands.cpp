#ifndef COMMANDS_DISASSEMBLY
#define COMMANDS_DISASSEMBLY

#include "commands.hpp"
#include "message.hpp"
#include <cstring>
#include <unistd.h>

using namespace std;

void Command_t::Execute(Binary_t&) { ; }

void MOV_I2R::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << ", ";

    printf("%02x", frame.decoded.data[1]);
    if(frame.decoded.w == 1)
        printf("%02x", frame.decoded.data[0]);
}
void RET_wSAI::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    printf("%02x%02x", frame.decoded.disp_high, frame.decoded.disp_low);
}
void IN_PORT::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("%02x", frame.decoded.port);
}
void IN_PORT_VAR::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << (frame.decoded.w == 1 ? "ax, " : "al, ");
    printf("dx");
}
void CALL_DS::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);
    printf("%04lx", frame.decoded.disp + pos + 3);
}
void CALL_IS::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    cout << regs_16[frame.decoded.rm];
}
void INT::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);

    std::cout << std::hex << (int)frame.raw[1];
}
void MOV_MwA::Disassemble(size_t pos) const
{
    Command_t::Disassemble(pos);
    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    printf("[%02x%02x]", frame.decoded.addr_high, frame.decoded.addr_low);
}

void CALL_DS::Execute(Binary_t& binary)
{
    // Mark return position to position after CALL
    // Push return address into stack
    binary.Push(binary.textPos + frame_length);

    // Move program to new position
    binary.JumpDS(frame.decoded.disp);
}
void CALL_IS::Execute(Binary_t& binary)
{
    // Mark return position to position after CALL
    binary.Push(binary.textPos + frame_length);

    uint16_t disp = binary.GetReg(1, frame.decoded.rm);

    // Move program to new position
    binary.JumpIS(disp - frame_length);
}
void MOV_I2R::Execute(Binary_t& binary)
{
    uint16_t value = frame.decoded.data[0];
    if(frame.decoded.w)
        value += (frame.decoded.data[1] << 8);

    binary.SetReg(frame.decoded.w, frame.decoded.reg, value);
}
void RET_wSAI::Execute(Binary_t& binary)
{
    binary.JumpIS(binary.Pop() - frame_length);

    binary.sp += (frame.decoded.disp_low | (frame.decoded.disp_high << 8));
}
void INT::Execute(Binary_t& binary)
{
    message* mess = (message*)&binary.stack[binary.bx];
    uint16_t addr, fd, request, length;
    int ret;
    if(LOG)
        printf("\n");
    switch(mess->m_type) {
    case 1: // exit
        if(LOG)
            printf("<exit(%d)>\n", mess->m_m1.m1i1);
        throw(int) mess->m_m1.m1i1;
        break;
    case 4: // write
        fd = mess->m_m1.m1i1;
        addr = mess->m_m1.m1p1;
        length = mess->m_m1.m1i2;
        if(LOG) {
            printf("<write(%d, 0x%04x, %d)", fd, addr, length);
            fflush(stdout);
        }
        ret = write(fd, &binary.stack[addr], length);
        mess->m_type = ret;
        if(LOG)
            printf(" => %d>", ret);

        binary.ax = 0;

        break;
    case 17: // brk
        addr = mess->m_m1.m1p1;
        binary.ax = 0;

        if(LOG)
            printf("<brk(0x%04x) => ", addr);

        if(addr < binary.dataSegmentSize ||
           (addr >= ((binary.sp & ~0x3ff) - 0x400))) {
            errno = ENOMEM;
            if(LOG)
                printf("ENOMEM>");

            mess->m_type = -errno;
        }
        else {
            if(LOG)
                printf("0>");

            mess->m_type = 0;
            mess->m_m1.m1p5 = addr;
        }

        break;
    case 54:
        fd = mess->m_m1.m1i1;
        request = mess->m_m1.m1i3;
        addr = mess->m_m1.m1p5;
        if(LOG)
            printf("<ioctl(%d, 0x%04x, 0x%04x)>", fd, request, addr);
        errno = EINVAL;
        mess->m_type = -errno;

        binary.ax = 0;

        break;
    default:
        printf("Unkown system call - %d\n", mess->m_type);
    }
}

Command_t::Command_t(uint8_t _frame_length, const char* _name)
    : frame_length{_frame_length}, name{_name}
{
}

void Command_t::PrintStatus(const Binary_t& binary) const
{
    binary.PrintStatus();
    Disassemble(binary.textPos);
}

void Command_t::Read(uint8_t* tab)
{
    for(int i = 0; i < frame_length; i++)
        SetFramePart(i, *(tab++));
}

void Command_t::Disassemble(size_t pos) const
{
    printf("%04lx: ", pos); // Print position

    char hexBuffer[14];
    hexBuffer[0] = '\0';
    for(int i = 0; i < frame_length; i++)
        sprintf(hexBuffer + strlen(hexBuffer), "%02x", GetFramePart(i));

    printf("%-*s %s ", 13, hexBuffer, name);
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
bool LOG;

#endif // COMMANDS_DISASSEMBLY