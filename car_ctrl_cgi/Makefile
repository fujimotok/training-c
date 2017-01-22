EXEC = car_ctrl.cgi
OBJS = car_ctrl.o

CROSS_COMPILE ?= arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -O3

LDLIBS := 

.PHONY: all romfs clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

romfs:
	$(ROMFSINST) /home/www-data/cgi-bin/$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS) 

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $<