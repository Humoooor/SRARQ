#ifndef TIMER_H
#define TIMER_H


struct Timer {
    pthread_t thread;
    int status;
};

#define TIMER_START 1
#define TIMER_STOP 2
// each timer is 1s
#define TIMEOUT 10

void InitTimer(uint8_t Rsize, int client_socket);
void StartTimer(uint8_t seqNo);
void StopTimer(uint8_t seqNo);
void TimeOut(void *seqNo);


#endif
