#include "singleton.hpp"

void Singleton::PrintCommand(size_t pos)
{
    Command_t::PrintCommand(pos);

    printf("\n");
}