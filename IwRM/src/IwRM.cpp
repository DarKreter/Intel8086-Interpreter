#include "IwRM.hpp"

using namespace std;

void I2RM_BASIC::PrintRM()
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
        offset += 2;
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            offset += 1;
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
            offset += 2;
            printf("+%x", (int)(frame.decoded.disp.d[0] +
                                (frame.decoded.disp.d[1] << 8)));
            break;
        case 0: // disp == 0
        default:
            break;
        }
        std::cout << "]";
    }
}
void I2RM_BASIC::Disassemble(size_t pos)
{
    frame_length = 3;
    if(frame.decoded.w == 1)
        frame_length += 1;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

    Command_t::Disassemble(pos);

    if(frame.decoded.w != 1 &&
       (frame.decoded.rm != 3 || frame.decoded.mod != 3))
        printf("byte ");

    PrintRM();

    if(frame.decoded.w == 1)
        printf(", %02x%02x\n", frame.decoded.disp.d[1 + offset],
               frame.decoded.disp.d[0 + offset]);
    else
        printf(", %x\n", frame.decoded.disp.d[0 + offset]);
}
void I2RM_BASIC_s::Disassemble(size_t pos)
{
    frame_length = 3;
    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        frame_length++;
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length += 2;
    else if(frame.decoded.mod == 1)
        frame_length++;

    Command_t::Disassemble(pos);

    if(frame.decoded.w != 1 &&
       (frame.decoded.rm != 1 || frame.decoded.mod != 3))
        printf("byte ");

    PrintRM();
    printf(", ");

    if(frame.decoded.s == 0 && frame.decoded.w == 1)
        printf("%02x%02x", frame.decoded.disp.d[1 + offset],
               frame.decoded.disp.d[0 + offset]);
    else {
        union {
            uint8_t u;
            int8_t i;
        } u;
        u.u = frame.decoded.disp.d[0 + offset];
        if(u.i < 0)
            printf("-%x", (int)-u.i);
        else
            printf("%x", (int)u.i);
    }
    printf("\n");
}