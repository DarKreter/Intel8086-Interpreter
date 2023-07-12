#include "message.hpp"
#include <cstdio>

message* sysCall(uint8_t* data, uint32_t disp)
{
    message* mess = (message*)&data[disp];
    // mess.m_source = data[disp] + (data[disp + 1] << 8);
    // mess.m_type = data[disp + 2] + (data[disp + 3] << 8);
    // union {
    //     uint8_t tab[2];
    //     uint16_t source;
    // };
    // for(int i = 0; i < 2; i++)
    //     // printf("%d", data[i + disp]);
    //     tab[i] = data[i + disp + 2];

    printf("source: %d\n", mess->m_source);
    printf("type: %d\n", mess->m_type);

    return mess;
}

// void MinixExit(uint16_t status)
// {

// }