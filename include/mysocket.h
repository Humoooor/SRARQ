#ifndef SOCKET_H
#define SOCKET_H

int CreateServerSocket(int port);
int WaitClient(int listen_socket);

int CreateClientSocket();
int ConnectServer(int client_socket, char *ip, int port);

#endif
