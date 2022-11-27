/*=============================================================================
#     FileName: frame.c
#         Desc: Operations on a frame
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-11-26 20:37:07
=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "types.h"
#include "mylog.h"
#include "check.h"

#define ESC 0x1b

// server

int ReceiveFrame(int client_socket, struct Frame *frame) {
    char buf;
    char data[MAXDATA];

    read(client_socket, &buf, 1);
    if(buf != FLAG) {
        return -1;
    }

    int len = 0;
    int esc = 0;

    memset(data, 0, MAXDATA);
    read(client_socket, &frame->seqNo, 1);
    while(read(client_socket, &buf, 1)) {
        if(len >= MAXDATA) {
            myWarnLog("Frame data is too long, the rest has been discarded");

            // get to the position of the latter FLAG
            while(read(client_socket, &buf, 1)) {
                if(esc) {
                    esc--;
                    continue;
                }
                if(buf == ESC) {
                    esc++;
                    continue;
                }
                if(buf == FLAG) {
                    break;
                }
            }
            break;
        }
        if(esc) {
            data[len] = buf;
            len++;
            esc--;
            continue;
        }
        if(buf == ESC) {
            esc++;
            continue;
        }
        if(buf == FLAG) {
            break;
        }
        data[len] = buf;
        len++;
    }

    // raw data contains CRC code
    frame->data = (char*)malloc(len - 1);
    for(int i = 0; i < len - 2; i++) {
        frame->data[i] = data[i];
    }
    frame->CRC = *(uint16_t*)(data + len - 2);

	return 0;
}

int CheckFrame(struct Frame *frame) {
    if(!CheckCRC16(frame->data, strlen(frame->data), frame->CRC)) {
        return 0;
    }
    return 1;
}

void SendACK(uint8_t seqNo, int client_socket) {
    struct ACK ack;
    ack.flag1 = FLAG;
    ack.type = TYP_ACK;
    ack.seqNo = seqNo;
    ack.flag2 = FLAG;

    write(client_socket, &ack, sizeof(ack));
}

void SendNAK(uint8_t seqNo, int client_socket) {
    struct ACK frame;
    frame.flag1 = FLAG;
    frame.type = TYP_NAK;
    frame.seqNo = seqNo;
    frame.flag2 = FLAG;

    write(client_socket, &frame, sizeof(frame));
}

//client

struct Frame *MakeFrame(int seqNo, char *data) {
	struct Frame *frame = (struct Frame*)malloc(sizeof(struct Frame));
    int escape_num = 0;

    for(int i = 0; i < strlen(data); ++i) {
        if(data[i] == FLAG) {
            escape_num++;
        }
    }

    char *frame_data = (char*)malloc(strlen(data) + escape_num + 1);
    for(int i = 0, j = 0; i < strlen(data); ++i, ++j) {
        if(data[i] == FLAG || data[i] == ESC) {
            frame_data[j] = ESC;
            ++j;
        }
        frame_data[j] = data[i];
    }
    frame_data[strlen(data) + escape_num] = 0;

    frame->flag1 = FLAG;
	frame->seqNo = seqNo;
    frame->data = frame_data;
	frame->CRC = CalcCRC16(data, strlen(data));
    frame->flag2 = FLAG;

	return frame;
}
