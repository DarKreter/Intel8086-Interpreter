#ifndef MESS_INT
#define MESS_INT

#include <cstdint>

#define M1 1
#define M3 3
#define M4 4
#define M3_STRING 14

typedef struct {
    uint16_t m1i1, m1i2, m1i3;
    uint16_t m1p1, m1p2, m1p3, m1p4, m1p5;
} mess_1;

typedef struct {
    uint16_t m_source; /* who sent the message */
    uint16_t m_type;   /* what kind of message is it */
    mess_1 m_m1;
} message;

#endif // MESS_INT