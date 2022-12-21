CC = gcc
root = $(CURDIR)
SRC_DIR = $(root)/src
INCL_DIR = $(root)/include
BUILD_DIR = $(root)/build
CFLAGS = -I$(INCL_DIR)

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
RECV_OBJ = $(foreach c, $(RECV_SRC:.c=.o), $(BUILD_DIR)/$(c))
SEND_HDR = $(HDRS) sendwindow.h
SEND_SRC = $(SRCS) sendwindow.c sender.c
SEND_OBJ = $(foreach c, $(SEND_SRC:.c=.o), $(BUILD_DIR)/$(c))

all: receiver sender

receiver: $(RECV_OBJ)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

sender: $(SEND_OBJ)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^

$(BUILD_DIR)/%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm $(BUILD_DIR)/* $(root)/*.o

.PHONY: clean all
