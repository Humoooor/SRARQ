/*=============================================================================
#     FileName: sender.c
#         Desc: Simulate the sender
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-11-28 16:25:48
=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "types.h"
#include "mylog.h"
#include "frame.h"
#include "mysocket.h"

#define IP "127.0.0.1"
#define PORT 7777

int main(int argc, char *argv[]) {
	struct Frame *frame;

	int client_socket = CreateClientSocket();
	int ret = ConnectServer(client_socket, IP, PORT);
    if(ret == 0) {
        myLog("Connected to %s %d", IP, PORT);
    } else {
        myErrLog("Connection failed");
    }


	for(int i = 0; i < 8; ++i) {
		frame = MakeFrame(i, "Test");
        SendFrame(client_socket, frame);
	}

    sleep(5);
	return 0;
}
