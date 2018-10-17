# chat-client
A chat client made in C 

## Compile it
You will need GTK+2.0: ```apt-get install gtk+-2.0```

Run ```gcc client.c -o client `pkg-config --cflags --libs gtk+-2.0` ``` on your Linux terminal. Then run ```./client```, or run ```make``` and then run ```./client```.

## Important methods
```void initConnection(GtkWidget *button, gpointer data)``` collects username, ip and port. This data has to be sent to server. Also, data received from request has to be saved here.

```void changeStatus(GtkWidget *combo, gpointer data)``` collects user status. This has to be sent to server.

```void sendMessage(GtkWidget *button, gpointer data)``` collects the current message sent by user. This has to be sent to server.

```void fetchUsers(gpointer data)``` has to make a request to server for userlist. Then you have to save user list to user array in the ChatClient struct. Also ```((ChatClient *)data)->totalUsers = 2;``` has to be changed to be equal to user list size after making the request. And all dummy data has to be wiped out. Same thing happens with ```void fetchMessages(gpointer data)```.
