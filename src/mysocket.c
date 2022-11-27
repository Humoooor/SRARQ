#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "mylog.h"

int CreateServerSocket(int port) {
	int ret;
	int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listen_socket == -1) {
		myErrLog("create socket error");
		exit(-1);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret = bind(listen_socket, (struct sockaddr*)&addr, sizeof(addr));
	if(ret == -1) {
		myErrLog("bind socket error");
		exit(-1);
	}

	// ret = listen(listen_socket, SOMAXCONN);
	ret = listen(listen_socket, 3);
	if(ret == -1) {
		myErrLog("listen socket error");
		exit(-1);
	}

	return listen_socket;
}

int WaitClient(int listen_socket) {
	struct sockaddr_in cliaddr;
	socklen_t len = sizeof(cliaddr);
	int client_socket = accept(listen_socket, (struct sockaddr*)&cliaddr, &len);

	if(client_socket == -1) {
		myErrLog("Accept error");
		exit(-1);
	}

	return client_socket;
}

int CreateClientSocket() {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket == -1) {
		exit(-1);
	}

	return client_socket;
}

int ConnectServer(int client_socket, char *ip, int port) {
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_aton(ip, &addr.sin_addr);

	int len = sizeof(addr);
	int listen_socket = connect(client_socket, (struct sockaddr*)&addr, len);
	if(listen_socket == -1) {
		exit(-1);
	}

	return listen_socket;
}
