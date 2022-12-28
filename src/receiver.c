/*=============================================================================
#     FileName: receiver.c
#         Desc: Simulate the receiver 
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-12-28 15:10:29
#      History:
=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "types.h"
#include "mylog.h"
#include "mysocket.h"
#include "frame.h"
#include "recvwindow.h"

#define PORT 7777

void DeliverData(uint8_t seqNo, char *data);

int main(int argc, char *argv[]) {
	int port = PORT;
	uint8_t Rn = 0;
	uint8_t seqNo;
	struct Frame *frame;
	char *data;
    int NAKSent = false;
    int ACKNeeded = false;

	if(argc > 2) {
		printf("Usage: server [port]\n");
		exit(-1);
	} else if(argc == 2) {
		port = atoi(argv[1]);
		if(port < 1024 || port > 65535) {
			myErrLog("error port");
			exit(-1);
		}
	}

    frame = (struct Frame*)malloc(sizeof(struct Frame));
    initWindow(RSIZE);

	int listen_socket = CreateServerSocket(port);
	myLog("Listening to port %d...", port);
	
	int client_socket = WaitClient(listen_socket);
	myLog("Connecting with a client");

	while(true) {
		int ret = ReceiveFrame(client_socket, frame);

        // check frame format
		if(ret == -1) {
            myErrLog("Wrong frame format");
			break;
        } else if(ret == 0) {
            myLog("Receive complished");
            break;
        }

		myLog("Receive frame %u", frame->seqNo);

        // check if frame is corrupted
		if(!CheckFrame(frame) && !NAKSent) {
			myErrLog("Frame %u corrupted", frame->seqNo);
            if(!NAKSent) {
                SendNAK(Rn, client_socket);
                myLog("NAK %u sent", Rn);
                NAKSent = true;
            }
			continue;
		}

        // for debug
        // myLog("Content len = %d, CRC = 0x%x", strlen(frame->data), frame->CRC);

        // check if frame is sent repeatedly
		if(isFrameExist(frame->seqNo)) {
			myWarnLog("Frame %u already exists", frame->seqNo);
			continue;
		}

        // if frame->seqNo is no equal to Rn, send Rn's NAK(if not sent yet)
        if(frame->seqNo != Rn && !NAKSent) {
            SendNAK(Rn, client_socket);
            NAKSent = true;
        }

        // check if in receiver window
		if(isInWindow(frame->seqNo, Rn)) {
			StoreFrame(frame);
		}

        // deliver data in sequence
		while(isFrameExist(Rn)) {
			data = ExtractData(Rn);
            myLog("Deliver frame %u", Rn);
			DeliverData(Rn, data);

			PurgeFrame(Rn);

			Rn++;
			Rn %= RSIZE;
            ACKNeeded = true;
		}

        if(ACKNeeded) {
            if(Rn > 0) {
                SendACK(Rn-1, client_socket);
                myLog("ACK %u sent", Rn-1);
            } else {
                SendACK(RSIZE-1, client_socket);
                myLog("ACK %u sent", RSIZE-1);
            }
            ACKNeeded = false;
            NAKSent = false;
        }

		//sleep(0.5);
	}

    free(frame);
    frame = NULL;
    close(client_socket);
	close(listen_socket);
	myLog("Connection close");

	return 0;
}

void DeliverData(uint8_t seqNo, char *data) {
    printf("-------- Frame %u ---------\n", seqNo);
    puts(data);
    puts("--------------------------");
}
