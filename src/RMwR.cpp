#include "RMwR.hpp"

void RMwR_BASIC::PrintRM()
{
    if(frame.decoded.mod == 0x03)
        // if mod == 11, rm is treated like reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.rm]
                                           : regs_16[frame.decoded.rm]);

    else if(frame.decoded.mod == 0 && frame.decoded.rm == 6) {
        printf("[%02x%02x]", frame.decoded.disp.d[1], frame.decoded.disp.d[0]);
    }
    else // mod == 00/01/10
    {
        std::cout << "[" << rm_memory[frame.decoded.rm];
        switch(frame.decoded.mod) {
        case 1:
            if(frame.decoded.disp.s < 0)
                printf("-%x", (int)-frame.decoded.disp.s);
            else
                printf("+%x", (int)frame.decoded.disp.s);
            break;
        case 2:
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

void RMwR_BASIC::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    printf("%s ", name);

    if(frame.decoded.d == 0) { // from reg
        PrintRM();

        std::cout << ", "
                  << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << std::endl;
    }
    else { // to reg
        std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                           : regs_16[frame.decoded.reg])
                  << ", ";

        PrintRM();

        std::cout << std::endl;
    }
}

void LEA::PrintCommand(size_t pos)
{
    if(frame.decoded.mod == 2 ||
       (frame.decoded.mod == 0 && frame.decoded.rm == 6))
        frame_length = 4;
    else if(frame.decoded.mod == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    printf("%s ", name);
    std::cout << (frame.decoded.w == 0 ? regs_8[frame.decoded.reg]
                                       : regs_16[frame.decoded.reg])
              << ", ";
    PrintRM();
    std::cout << std::endl;
}