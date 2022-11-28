/*=============================================================================
#     FileName: recvwindow.c
#         Desc: Operations on receiver window
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-11-27 16:55:30
=============================================================================*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

static char **datas;
static uint8_t seq_max;
static uint8_t window_size;

void initWindow(uint8_t Rsize) {
    seq_max = Rsize - 1;
    window_size = Rsize / 2;
    datas = (char**)malloc(sizeof(char*) * Rsize);
    memset(datas, 0, sizeof(char*) * Rsize);
}

int isFrameExist(uint8_t seqNo) {
    return datas[seqNo] != NULL; 
}

int isInWindow(uint8_t seqNo, uint8_t Rn) {
    uint8_t left = Rn;
    uint8_t right = (Rn + window_size - 1) % (seq_max + 1);
    int res = 0;

    if(left < right && seqNo >= left && seqNo <= right) {
        res = 1;
    } else if(left > right && (seqNo >= left || seqNo <= right)) {
        res = 1;
    }

    return res;
}

void StoreFrame(struct Frame *frame) {
    datas[frame->seqNo] = frame->data;
}

void PurgeFrame(uint8_t seqNo) {
    free(datas[seqNo]);
    datas[seqNo] = NULL;
}

char *ExtractData(uint8_t seqNo) {
    return datas[seqNo];
}
