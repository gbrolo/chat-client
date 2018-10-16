#gcc client.c -o client `pkg-config --cflags --libs gtk+-2.0`
#Makefile

CC = gcc
CFLAGS = -I -g -Wall
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)
objects = client.o


client: client.c
	$(CC) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs gtk+-2.0` 


chat : $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) -o client $(objects)

.PHONY : clean
clean :
	-rm client $(objects)
