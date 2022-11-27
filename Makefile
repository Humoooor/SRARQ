root = $(CURDIR)
SRC_DIR = $(root)/src
INCLUDE_DIR = $(root)/include
BUILD_DIR = $(root)/build
recv_obj = 
send_obj = 
CC = gcc




Receiver: error.h types.h
	$(CC) -o receiver $(re_obj)

Sender: error.h types.h
	$(CC) -o sender $(se_obj)

clean:
	-rm receiver sender

.PHONY: clean
