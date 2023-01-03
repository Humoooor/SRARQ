#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "types.h"
#include "frame.h"
#include "timer.h"
#include "mylog.h"
#include "sendwindow.h"

static int client_socket;
static struct Timer *timers;

void InitTimer(uint8_t Rsize, int socket) {
    timers = (struct Timer*)malloc(Rsize * sizeof(struct Timer));
    client_socket = socket;
}

void StartTimer(uint8_t seqNo) {
    StopTimer(seqNo);
    pthread_join(timers[seqNo].thread, NULL);
    timers[seqNo].status = TIMER_START;

    // timeout
    pthread_create(&timers[seqNo].thread, NULL, (void*)TimeOut, (void*)&seqNo);
    // wait for pass params to TimeOut
    usleep(UNIT_TIME(1));
}

void StopTimer(uint8_t seqNo) {
    timers[seqNo].status = TIMER_STOP;
}

void TimeOut(void *seqNo) {
    uint8_t n = *(uint8_t*)seqNo;
    struct Frame *frame;

    // restart until timer stop
    while(true) {
        // timer may stop
        for(int i = 0; i < TIMEOUT && timers[n].status == TIMER_START; i++) {
            usleep(UNIT_TIME(1));
        }
        // if stop, do not resend frame
        if(timers[n].status == TIMER_STOP) {
            break;
        }
        myLog("Frame %u timeout", n);
        frame = GetFrame(n);
        SendFrame(client_socket, frame);
        myLog("Frame %u resent", n);
    }

    pthread_exit(0);
}
