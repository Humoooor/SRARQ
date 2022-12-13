CC = gcc
root = $(CURDIR)
SRC_DIR = $(root)/src
INCL_DIR = $(root)/include
CFLAGS = -I$(INCL_DIR)

HDRS = $(INCL_DIR)/check.h \
	   $(INCL_DIR)/frame.h \
	   $(INCL_DIR)/mylog.h \
	   $(INCL_DIR)/mysocket.h \
	   $(INCL_DIR)/types.h \

SRCS = $(SRC_DIR)/check.c \
	   $(SRC_DIR)/frame.c \
	   $(SRC_DIR)/mylog.c \
	   $(SRC_DIR)/mysocket.c \

RECV_HDR = $(HDRS) $(INCL_DIR)/recvwindow.h
RECV_SRC = $(SRCS) $(SRC_DIR)/recvwindow.c $(SRC_DIR)/receiver.c
RECV_OBJ = $(RECV_SRC:.c=.o)
SEND_HDR = $(HDRS) $(INCL_DIR)/sendwindow.h
SEND_SRC = $(SRCS) $(SRC_DIR)/sendwindow $(SRC_DIR)/sender.c
SEND_OBJ = $(SEND_SRC:.c=.o)

all: receiver sender

receiver: $(RECV_OBJ)
	$(CC) $(CFLAGS) -o $(root)/receiver $(RECV_OBJ)

sender: $(SEND_OBJ)
	$(CC) $(CFLAGS) -o $(root)/sender $(SEND_OBJ)

.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(SRC_DIR)/*.o $(root)/receiver $(root)/sender

.PHONY: clean
