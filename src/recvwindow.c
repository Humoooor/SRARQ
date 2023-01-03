#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define UP "┌─┐"
#define DOWN "└─┘"
#define GREEN "\033[32m"
#define LIGHTGREEN "\033[92m"
#define END "\033[0m"

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

void PrintFrame(uint8_t Rn) {

    for(int i = 0; i <= seq_max; i++) {
        // add color
        if(i == Rn) {
            printf(LIGHTGREEN);
        } else if(datas[i]) {
            printf(GREEN);
        }

        printf(UP);
        printf(END);
    }
    printf("\n");

    for(int i = 0; i <= seq_max; i++) {
        // add color
        if(i == Rn) {
            printf(LIGHTGREEN);
        } else if(datas[i]) {
            printf(GREEN);
        }
        printf(DOWN);
        printf(END);
    }
    printf("\n");
}
