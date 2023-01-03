#ifndef SENDWINDOW_H
#define SENDWINDOW_H

void InitWindow(uint8_t Rsize);
int isBetween(uint8_t left, uint8_t right, uint8_t seqNo);
int isInWindow(uint8_t Sf, uint8_t seqNo);
void StoreFrame(struct Frame *frame);
void PurgeFrame(uint8_t seqNo);
struct Frame* GetFrame(uint8_t seqNo);
void PrintFrame(uint8_t Sf, uint8_t Sn);

#endif
