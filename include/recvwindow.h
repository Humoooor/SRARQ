#ifndef RECVWINDOW_H
#define RECVWINDOW_H

void initWindow(uint8_t Rsize);
int isInWindow(uint8_t seqNo, uint8_t Rn);
int isFrameExist(uint8_t seqNo);
void StoreFrame(struct Frame *frame);
void PurgeFrame(uint8_t seqNo);
char *ExtractData(uint8_t seqNo);

#endif
