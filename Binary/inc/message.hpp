#ifndef MESS_INT
#define MESS_INT

#include <cstdint>
/* Types relating to messages. */
#define M1 1
#define M3 3
#define M4 4
#define M3_STRING 14

typedef struct {
    uint16_t m1i1, m1i2, m1i3;
    uint16_t m1p1, m1p2, m1p3;
} mess_1;
typedef struct {
    int m2i1, m2i2, m2i3;
    long m2l1, m2l2;
    char* m2p1;
} mess_2;
typedef struct {
    int m3i1, m3i2;
    char* m3p1;
    char m3ca1[M3_STRING];
} mess_3;
typedef struct {
    uint32_t m4l1, m4l2, m4l3, m4l4, m4l5;
} mess_4;
typedef struct {
    char m5c1, m5c2;
    int m5i1, m5i2;
    long m5l1, m5l2, m5l3;
} mess_5;
typedef struct {
    int m6i1, m6i2, m6i3;
    long m6l1;
    uint16_t m6f1;
} mess_6;

typedef struct {
    uint16_t m_source; /* who sent the message */
    uint16_t m_type;   /* what kind of message is it */
    mess_1 m_m1;
    // union {
    //     mess_1 m_m1;
    //     mess_2 m_m2;
    //     mess_3 m_m3;
    //     mess_4 m_m4;
    //     mess_5 m_m5;
    //     mess_6 m_m6;
    // } m_u;
} message;

message* sysCall(uint8_t* data, uint32_t disp);
// void MinixExit(uint16_t status);

#endif // MESS_INT