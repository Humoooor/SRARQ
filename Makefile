CC = gcc
root = $(CURDIR)
SRC_DIR = $(root)/src
INCL_DIR = $(root)/include
BUILD_DIR = $(root)/build
CFLAGS = -I$(INCL_DIR) -lpthread

vpath %.c $(SRC_DIR)
vpath %.h $(INCL_DIR)
vpath %.o $(BUILD_DIR)

HDRS = check.h \
       frame.h \
       mylog.h \
       mysocket.h \
       types.h \

SRCS = check.c \
       frame.c \
       mylog.c \
       mysocket.c \

RECV_HDR = $(HDRS) recvwindow.h
RECV_SRC = $(SRCS) recvwindow.c receiver.c
RECV_OBJ = $(addprefix $(BUILD_DIR)/, $(RECV_SRC:.c=.o))
SEND_HDR = $(HDRS) sendwindow.h timer.h
SEND_SRC = $(SRCS) sendwindow.c timer.c sender.c
SEND_OBJ = $(addprefix $(BUILD_DIR)/, $(SEND_SRC:.c=.o))

all: receiver sender

receiver: $(RECV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

sender: $(SEND_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(BUILD_DIR)/* $(root)/receiver $(root)/sender

.PHONY: clean all
