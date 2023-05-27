#include "IwRM.hpp"

void I2RM_BASIC::PrintCommand(size_t pos)
{
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        frame_length = 4;
    else
        frame_length = 3;

    Command_t::PrintCommand(pos);

    printf("%s ", name);

    if(frame.decoded.mod == 0x03) // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        printf(", %02x%02x\n", frame.decoded.data[0], frame.decoded.data[1]);
    else
        printf(", %x\n", frame.decoded.data[0]);
}