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
#define SendField(socket, x) write(socket, &(x), sizeof(x))

// server

int ReceiveFrame(int client_socket, struct Frame *frame) {
    int ret;
    char buf;
    char raw_data[MAXDATA + 2];

    ret = read(client_socket, &buf, 1);
    if(ret != 0 && buf != FLAG) {
        return -1;
    } else if(ret == 0) {
        // socket closed
        return 0;
    }

    int len = 0;
    int esc = 0;

    memset(raw_data, 0, sizeof(raw_data));
    read(client_socket, &frame->seqNo, 1);
    while(ret = read(client_socket, &buf, 1)) {
        if(len >= MAXDATA + 2) {
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
            raw_data[len] = buf;
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
        raw_data[len] = buf;
        len++;
    }

    // raw_data contains CRC code(2 Bytes)
    frame->data = (char*)malloc(len - 2 + 1);
    memset(frame->data, 0, len - 2 + 1);
    for(int i = 0; i < len - 2; i++) {
        frame->data[i] = raw_data[i];
    }
    frame->CRC = *(uint16_t*)(raw_data + len - 2);

	return ret;
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

// client

struct Frame *MakeFrame(int seqNo, char *data) {
	struct Frame *frame = (struct Frame*)malloc(sizeof(struct Frame));
    int escape_num = 0;

    for(int i = 0; i < strlen(data); ++i) {
        if(data[i] == FLAG || data[i] == ESC) {
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

    free(data);
	return frame;
}

void SendFrame(int client_socket, struct Frame *frame) {
    SendField(client_socket, frame->flag1);
    SendField(client_socket, frame->seqNo);
    write(client_socket, frame->data, strlen(frame->data));
    SendField(client_socket, frame->CRC);
    SendField(client_socket, frame->flag2);
}

int ReceiveACK(int client_socket, struct ACK *ack) {
    int ret;
    char buf;

    read(client_socket, &buf, 1);
    if(buf != FLAG) {
        return -1;
    }
    read(client_socket, &ack->type, sizeof(ack->type));
    read(client_socket, &ack->seqNo, sizeof(ack->seqNo));
    read(client_socket, &buf, 1);
    if(buf != FLAG) {
        return -1;
    }

    return 0;
}
