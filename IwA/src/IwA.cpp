#include "IwA.hpp"

void IwA::PrintCommand(size_t pos)
{
    if(frame.decoded.w == 1)
        frame_length = 3;
    else
        frame_length = 2;

    Command_t::PrintCommand(pos);

    if(frame.decoded.w == 0)
        printf("al, ");
    else
        printf("ax, ");

    if(frame.decoded.w == 1)
        printf("%02x%02x\n", frame.decoded.data[1], frame.decoded.data[0]);
    else
        printf("%x\n", frame.decoded.data[0]);
}