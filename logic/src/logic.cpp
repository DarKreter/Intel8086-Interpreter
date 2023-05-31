#include "logic.hpp"

void LGC_BASIC::PrintBase(size_t pos)
{
    if(frame.decoded.mod == 0 && frame.decoded.rm == 6)
        frame_length = 4;
    else
        frame_length = 2;

    Command_t::Disassemble(pos);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);
}
void LGC_BASIC::Disassemble(size_t pos)
{
    PrintBase(pos);

    printf(", %s\n", frame.decoded.v == 0 ? "1" : "cl");
}
void DIV::Disassemble(size_t pos)
{
    PrintBase(pos);
    printf("\n");
}