/*=============================================================================
#     FileName: sender.c
#         Desc: 
#       Author: Humoooor
#        Email: humoooor@qq.com
#     HomePage: https://humoooor.cn
#      Version: 0.0.1
#   LastChange: 2022-11-21 22:58:23
=============================================================================*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "frame.h"
#include "mysocket.h"

int main(int argc, char *argv[]) {
	int client_socket = CreateClientSocket();
	int listen_socket = ConnectServer(client_socket, "127.0.0.1", 7777);

	struct Frame *frame;

	for(int i = 1; i < 10; ++i) {
		frame = MakeFrame(i, "Test");
		write(client_socket, frame, sizeof(struct Frame));
	}
	frame = MakeFrame(0, "Test");
	write(client_socket, frame, sizeof(struct Frame));

	return 0;
}
