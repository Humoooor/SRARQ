#ifndef TYPES_H
#define TYPES_H


struct Frame {
    uint8_t flag1;
    uint8_t seqNo;
    char *data;
    uint16_t CRC;
    uint8_t flag2;
};

struct ACK {
    uint8_t flag1;
    uint8_t type;
    uint8_t seqNo;
    uint8_t flag2;
};

#define FLAG 0b01111110
#define MAXDATA 0x100
#define TYP_ACK 1
#define TYP_NAK 2
#define RSIZE 8

// use usleep(t), the unit time is 100ms
#define UNIT_TIME(t) (t * 100000)


#endif
