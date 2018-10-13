#gcc client.c -o client `pkg-config --cflags --libs gtk+-2.0`
#Makefile

CC = gcc
CFLAGS = -I -g

client: client.c
	$(CC) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs gtk+-2.0`
