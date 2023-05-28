#include "jmp.hpp"

using namespace std;

void JMP_BASIC::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);
    // + 2, because actual position is after jump(jump is 2 byte long)
    printf("%s %04x\n", name, (int)(frame.decoded.disp + pos + 2));
}
void JMP_DS::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("jmp %04x\n", (uint16_t)(frame.decoded.disp_low +
                                     (frame.decoded.disp_high << 8) + pos + 3));
}