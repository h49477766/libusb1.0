
CC = gcc
AR = ar
#LIBPATH = libusb1.0
LIBPATH = /usr/local/lib
TARGET = test
CFLAGS += -I $(LIBPATH) -I /usr/local/include/libusb-1.0
LDCFLSGS += -lusb-1.0 
src := $(wildcard  *.c) 
OBJS = $(patsubst %.c,%.o, $(src))
$(TARGET):$(OBJS)
	#make -C $(LIBPATH)
	$(CC)  -o $@ $^  $(CFLAGS) $(LDCFLSGS) -g

clean:
	#make -C $(LIBPATH) clean
	-rm $(TARGET) *.o *.a
