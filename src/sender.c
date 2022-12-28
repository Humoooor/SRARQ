/*=============================================================================
#     FileName: sender.c
#         Desc: Simulate the sender
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-12-27 19:15:37
=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "types.h"
#include "mylog.h"
#include "frame.h"
#include "mysocket.h"
#include "sendwindow.h"
#include "timer.h"

#define IP "127.0.0.1"
#define PORT 7777

char* GetRandData(); 
void ACKHandle(void *client_socket);

static uint8_t Sf = 0;
static uint8_t Sn = 0;

int main(int argc, char *argv[]) {
    struct Frame *frame;
    char *data;
    int window_full = 0;

    // connect to receiver
    int client_socket = CreateClientSocket();
    int ret = ConnectServer(client_socket, IP, PORT);
    if(ret == 0) {
        myLog("Connected to %s %d", IP, PORT);
    } else {
        myErrLog("Connection failed");
    }

    // initialize random seed
    time_t t;
    srand((unsigned)time(&t));

    InitWindow(RSIZE);
    InitTimer(RSIZE, client_socket);

    pthread_t ACKhandle;
    pthread_create(&ACKhandle, NULL, ACKHandle, (void*)&client_socket);

    myLog("Sender start");
    while(true) {
        // send frame every 200ms
        usleep(UNIT_TIME(2));
        data = GetRandData();
        frame = MakeFrame(Sn, data);

        // sleep
        while(!isInWindow(Sf, Sn)) {
            if(window_full == 0) {
                myLog("Sender window full");
                window_full = 1;
            }
        }
        window_full = 0;

        StoreFrame(frame);
        SendFrame(client_socket, frame);
        StartTimer(Sn);
        myLog("Frame %u sent", Sn);
        myLog("Frame %u timer start", Sn);
        Sn++;
        Sn %= RSIZE;
    }

	return 0;
}

// get random string for test
char* GetRandData() {
    int n;
    char buf;
    char *data;

    n = rand() % (MAXDATA - 1) + 1;
    // 1 reserved for '\0'
    data = (char*)malloc(n+1);
    memset(data, 0, n+1);
    for(int i = 0; i < n; i++) {
        // get printable char
        buf = rand() % (0x7e - 0x20 + 1) + 0x20;
        data[i] = buf;
    }
    return data;
}

void ACKHandle(void *socket) {
    int client_socket = *(int*)socket;
    struct ACK *ack = (struct ACK*)malloc(sizeof(struct ACK));
    struct Frame *frame;

    while(true) {
        int ret = ReceiveACK(client_socket, ack);

        if(ret == -1) {
            myWarnLog("ACK/NAK corrupted");
            // sleep
            continue;
        }

        if(!isBetween(Sf, Sn, ack->seqNo)) {
            myWarnLog("ACK/NAK %u not between Sf and Sn", ack->seqNo);
            continue;
        }

        if(ack->type == TYP_ACK) {
            myLog("ACK %u received", ack->seqNo);
            while(isBetween(Sf, Sn, ack->seqNo)) {
                StopTimer(Sf);
                PurgeFrame(Sf);
                Sf++;
                Sf %= RSIZE;
                usleep(UNIT_TIME(2));
            }
            myLog("Sf move to %u", Sf);
        }

        if(ack->type == TYP_NAK) {
            usleep(UNIT_TIME(2));
            myLog("NAK %u received", ack->seqNo);
            // resend frame
            frame = GetFrame(ack->seqNo);
            SendFrame(client_socket, frame);
            frame = NULL;
            myLog("Frame %u resent", ack->seqNo);
            StartTimer(ack->seqNo);
        }
    }

    pthread_exit(0);
}

