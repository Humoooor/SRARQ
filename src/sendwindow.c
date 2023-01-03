#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "types.h"
#include "frame.h"
#include "sendwindow.h"

#define UP "┌─┐"
#define DOWN "└─┘"
#define GREEN "\033[92m"
#define BLUE "\033[34m"
#define END "\033[0m"

static struct Frame **frames;
static uint8_t seq_max;
static uint8_t window_size;

void InitWindow(uint8_t Rsize) {
    seq_max = Rsize - 1;
    window_size = Rsize / 2;
    frames = (struct Frame**)malloc(Rsize * sizeof(struct Frame*));
}

int isBetween(uint8_t left, uint8_t right, uint8_t seqNo){
    int res = 0;

    if(left <= right && left <= seqNo && seqNo <= right) {
        res = 1;
    } else if(left > right && (seqNo >= left || seqNo <= right)) {
        res = 1;
    }

    return res;
}

int isInWindow(uint8_t Sf, uint8_t seqNo) {
    uint8_t left = Sf;
    uint8_t right = (Sf + window_size - 1) % (seq_max + 1);

    return isBetween(left, right, seqNo);
}

void StoreFrame(struct Frame *frame) {
    frames[frame->seqNo] = frame;
}

void PurgeFrame(uint8_t seqNo) {
    free(frames[seqNo]->data);
    frames[seqNo]->data = NULL;
    free(frames[seqNo]);
    frames[seqNo] = NULL;
}

struct Frame* GetFrame(uint8_t seqNo) {
    return frames[seqNo];
}

void PrintFrame(uint8_t Sf, uint8_t Sn) {
    for(int i = 0; i <= seq_max; i++) {
        // add color
        if(i == Sn) {
            printf(BLUE);
        } else if(isBetween(Sf, Sn, i)) {
            printf(GREEN);
        }

        printf(UP);
        printf(END);
    }
    printf("\n");

    for(int i = 0; i <= seq_max; i++) {
        // add color
        if(i == Sn) {
            printf(BLUE);
        } else if(isBetween(Sf, Sn, i)) {
            printf(GREEN);
        }

        printf(DOWN);
        printf(END);
    }
    printf("\n");
}
