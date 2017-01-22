EXEC = sample_convert
OBJS = sample_convert.o

CROSS_COMPILE ?= arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -O3

LDLIBS := -lopencv_core -lopencv_highgui -lopencv_imgproc -lm

.PHONY: all romfs clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) /usr/bin/$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS) *.bmp

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<