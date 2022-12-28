#ifndef FRAME_H
#define FRAME_H

// server
int ReceiveFrame(int client_socket,  struct Frame *frame);
int CheckFrame(struct Frame *frame);
void SendACK(uint8_t seqNo, int client_socket);
void SendNAK(uint8_t seqNo, int client_socket);

// client
struct Frame *MakeFrame(int seqNo, char *data);
void SendFrame(int client_socket, struct Frame *frame);
int ReceiveACK(int client_socket, struct ACK *ack);

#endif
