#ifndef CHECK_H
#define CHECK_H

uint16_t CalcCRC16(char const *data, int len);
int CheckCRC16(char const *data, int len, uint16_t CRC);

#endif
