#include <gtk/gtk.h>
#include <gmodule.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>



// message struct: this will hold messages provided by server
typedef struct {
  char *from;
  char *message;
  char *action;
} message;

typedef struct {
  char *id;
  char *name;
  char *status;
} user;

typedef struct chat_client_ui {
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *vMainBox;
  GtkWidget *vFriendsBox;
  GtkWidget *hInputBox;
  GtkWidget *menubar;
  GtkWidget *fileMenu;
  GtkWidget *aboutMenu;
  GtkWidget *fileMi;
  GtkWidget *quitMi;
  GtkWidget *aboutMi;
  GtkWidget *helpMi;
  GtkWidget *sendBtn;
  GtkWidget *chatEntry;
  GtkWidget *statusCombo;
  GtkWidget *hBox;
  GtkWidget *friendsLabel;
  GtkWidget *messagesScrollWindow;
  GtkWidget *vChatBox;
  GtkWidget *friendsScrollWindow;
  GtkWidget *vFriendsBoxView;
  GtkWidget *hFriendInfoBox;
  GtkWidget *friendInfoBtn;
  GtkWidget *friendSendChatBtn;
  GtkWidget *msg;
  GtkWidget *chatMsg;
  GtkWidget *statusLbl;
  GtkWidget *infoHBox;
  GtkWidget *infoLabelHBox;
  GtkWidget *userEntry;
  GtkWidget *portEntry;
  GtkWidget *ipEntry;
  GtkWidget *sendInfoBtn;
  GtkWidget *connectionLbl;
  int totalMessages;
  int totalUsers;
  char buffer[32];
  char userId[32];
  char *currentStatus;
  message messages[500];
  user users[50];
  int i, j;
} ChatClient;

void showHelp(GtkWidget *widget, gpointer window) {
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(window,
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		"First add your username, the IP address for server and port in the upper inputs. \nSend messages using the bottom input and clicking on \"Send\" button.\nChange your status using the contiguous combobox.\nYou can send messages to active users by clicking on the buttons with their names on the right.");
  gtk_window_set_title(GTK_WINDOW(dialog), "Help");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void initConnection(GtkWidget *button, gpointer data) {
  char username[32];
  char ip[32];
  char port[32];

  sprintf(username, "%s", gtk_entry_get_text(GTK_ENTRY(((ChatClient *)data)->userEntry)));  
  sprintf(ip, "%s", gtk_entry_get_text(GTK_ENTRY(((ChatClient *)data)->ipEntry)));
  sprintf(port, "%s", gtk_entry_get_text(GTK_ENTRY(((ChatClient *)data)->portEntry))); 

  sprintf(((ChatClient *)data)->userId, "2"); 

  g_print("%s, %s, %s\n", username, ip, port);
}

void changeStatus(GtkWidget *combo, gpointer data) {
  char comboBuffer[32];
  char labelBuffer[32];
  sprintf(comboBuffer, "%s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo)));
  sprintf(labelBuffer, "You are %s", gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo)));
  gtk_label_set_text(GTK_LABEL(((ChatClient *)data)->statusLbl),labelBuffer);
  // send status with comboBuffer
  g_print("%s\n", comboBuffer);
}

// here we need to send the message to server
void sendMessage(GtkWidget *button, gpointer data) {
  char entryBuffer[32];
  sprintf(entryBuffer, "%s", gtk_entry_get_text(GTK_ENTRY(((ChatClient *)data)->chatEntry)));
  sprintf(((ChatClient *)data)->buffer, "You: %s\n", gtk_entry_get_text(GTK_ENTRY(((ChatClient *)data)->chatEntry)));

  ((ChatClient *)data)->chatMsg = gtk_label_new(((ChatClient *)data)->buffer);
  gtk_misc_set_alignment(GTK_MISC(((ChatClient *)data)->chatMsg), 0.0, 0.5);
  gtk_box_pack_start(GTK_BOX(((ChatClient *)data)->vChatBox), ((ChatClient *)data)->chatMsg, FALSE, FALSE, 0);

  // send message with entryBuffer
  g_print("%s\n", entryBuffer);
  g_print("%s\n", ((ChatClient *)data)->buffer);

  gtk_widget_show_all(((ChatClient *)data)->window);
}

// Here we need to make the call to the server to fill up messages list
void renderMessages(GtkWidget *widget, gpointer data){
  int i = ((ChatClient *)data)->i;
  char userIdBuffer[32];  

  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(((ChatClient *)data)->vChatBox));
  for (iter = children; iter != NULL; iter = g_list_next(iter))
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  g_list_free(children);

  // find out what is the id of the user sending the messages
  // sizeof(((ChatClient *)data)->users)/sizeof(((ChatClient *)data)->users[0])
  for (i = 0; i < ((ChatClient *)data)->totalUsers; i++) {
    if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), ((ChatClient *)data)->users[i].name) == 0) {
      sprintf(userIdBuffer, "%s", ((ChatClient *)data)->users[i].id);
    }
  }

  // sizeof(((ChatClient *)data)->messages)/sizeof(((ChatClient *)data)->messages[0]) 
  for (i = 0; i < ((ChatClient *)data)->totalMessages; i++) {
    // gtk_button_get_label(GTK_BUTTON(widget))    ((ChatClient *)data)->messages[i].from
    if ((strcmp(userIdBuffer, ((ChatClient *)data)->messages[i].from) == 0)) {
        sprintf(((ChatClient *)data)->buffer, "%s: %s\n", 
            gtk_button_get_label(GTK_BUTTON(widget)), ((ChatClient *)data)->messages[i].message);
        ((ChatClient *)data)->chatMsg = gtk_label_new(((ChatClient *)data)->buffer);
        gtk_misc_set_alignment(GTK_MISC(((ChatClient *)data)->chatMsg), 0.0, 0.5);
        gtk_box_pack_start(GTK_BOX(((ChatClient *)data)->vChatBox), ((ChatClient *)data)->chatMsg, FALSE, FALSE, 0);
        g_print("%s: %s\n", ((ChatClient *)data)->messages[i].from, ((ChatClient *)data)->messages[i].message);
        g_print("chatMsg %s", gtk_label_get_text(GTK_LABEL(((ChatClient *)data)->chatMsg)));
    }    
  }

  gtk_widget_show_all(((ChatClient *)data)->window);
}

// make connection to server here
void fetchUsers(gpointer data) {
  ((ChatClient *)data)->totalUsers = 2;
  user usr1, usr2;
  usr1.id = "0";
  usr1.name = "user_1";
  usr1.status = "active";
  ((ChatClient *)data)->users[0] = usr1;
  usr2.id = "1"; 
  usr2.name = "user_2";
  usr2.status = "busy";
  ((ChatClient *)data)->users[1] = usr2;  
}

// make connection to server here
void fetchMessages(gpointer data) {  
  ((ChatClient *)data)->totalMessages = 2;
  message msg1, msg2;
  msg1.from = "0";
  msg1.message = "msg_1";
  ((ChatClient *)data)->messages[0] = msg1;
  msg2.from = "1";
  msg2.message = "msg_2";
  ((ChatClient *)data)->messages[1] = msg2;
}

int main(int argc, char *argv[]) {
  int sockfd;
  char buffer[1000];
  char server_reply[2000];
  ssize_t n;

//=======================0.0.0.0===============================
  int fd;
  struct ifreq ifr;
  fd = socket(AF_INET, SOCK_DGRAM, 0);
 /* I want to get an IPv4 IP address */
  ifr.ifr_addr.sa_family = AF_INET;
 /* I want IP address attached to "eth0" */
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
 ioctl(fd, SIOCGIFADDR, &ifr);
 close(fd);
 /* display result */
 printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
//=======================0.0.0.0===============================

  //Funcion en desarrollo, obtencion de datos del usuario y servidor al que se va a conectar
  char *port; 
  char *ip; 
  char *username;

  size_t raw_length; 
  int length = 0; 
  char * hostSol = "{\"host\": "; 
  char * originSol = ", \"origin\" : "; 
  char * userSol = ", \"user\" : ";
  char * endJson = "}"; 

  //Get my IP
  char *myIP = INADDR_ANY;  

  // printf("%zu\n", raw_length);
  // Unica forma de imprimir un size_t

  //Enviar string requestID para inicial :D 
  //Se espera recibir mensaje que contenga string de json
  //convertirlo a un json object
  //iterarlo y buscar el id que envio el server :D 


  //---------------------------


  ChatClient chat;
  int i = chat.i;
  int j = chat.j;

  // temporary users creation for testing
  fetchUsers(&chat);

  // temporary message creation for testing
  fetchMessages(&chat);

  gtk_init(&argc, &argv);

  // initial user status
  chat.currentStatus = "active";

  // main window
  chat.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(chat.window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(chat.window), 600, 400);
  gtk_window_set_title(GTK_WINDOW(chat.window), "Chactter");

  // scrollable window for messages
  chat.messagesScrollWindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (chat.messagesScrollWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_widget_set_size_request(chat.messagesScrollWindow, 300, 250);

  // scrollable window for users in chatroom
  chat.friendsScrollWindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (chat.friendsScrollWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_widget_set_size_request(chat.friendsScrollWindow, 300, 300);
  
  // boxes
  chat.vbox = gtk_vbox_new(FALSE, 0);
  chat.vMainBox = gtk_vbox_new(FALSE, 0);
  chat.vChatBox = gtk_vbox_new(FALSE, 0);
  chat.vFriendsBox = gtk_vbox_new(FALSE, 0);
  chat.vFriendsBoxView = gtk_vbox_new(FALSE, 0);  
  chat.hInputBox = gtk_hbox_new(FALSE, 0);
  chat.infoHBox = gtk_hbox_new(FALSE, 0);
  chat.infoLabelHBox = gtk_hbox_new(FALSE, 0);
  chat.hBox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(chat.window), chat.vbox);

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(chat.messagesScrollWindow), chat.vChatBox);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(chat.friendsScrollWindow), chat.vFriendsBoxView);

  // top menu elements
  chat.menubar = gtk_menu_bar_new();
  chat.fileMenu = gtk_menu_new();
  chat.aboutMenu = gtk_menu_new();

  chat.fileMi = gtk_menu_item_new_with_label("File");
  chat.quitMi = gtk_menu_item_new_with_label("Quit");
  chat.aboutMi = gtk_menu_item_new_with_label("About");
  chat.helpMi = gtk_menu_item_new_with_label("Help");

  // GTK widgets
  chat.sendBtn = gtk_button_new_with_label("Send");
  g_signal_connect(GTK_OBJECT(chat.sendBtn), "clicked", GTK_SIGNAL_FUNC(sendMessage), &chat);

  chat.sendInfoBtn = gtk_button_new_with_label("Connect"); 
  g_signal_connect(GTK_OBJECT(chat.sendInfoBtn), "clicked", GTK_SIGNAL_FUNC(initConnection), &chat);

  chat.chatEntry = gtk_entry_new();
  chat.userEntry = gtk_entry_new();
  chat.portEntry = gtk_entry_new();
  chat.ipEntry = gtk_entry_new();

  chat.friendsLabel = gtk_label_new("Connected people");
  chat.connectionLbl = gtk_label_new("Username, IP address, port");

  chat.statusLbl = gtk_label_new("You are active");

  chat.statusCombo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(chat.statusCombo), "Active");
  gtk_combo_box_append_text(GTK_COMBO_BOX(chat.statusCombo), "Busy");
  gtk_combo_box_append_text(GTK_COMBO_BOX(chat.statusCombo), "Inactive");

  g_signal_connect(G_OBJECT(chat.statusCombo), "changed", G_CALLBACK(changeStatus), &chat);

  // widget placement
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(chat.fileMi), chat.fileMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(chat.aboutMi), chat.aboutMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(chat.fileMenu), chat.quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(chat.aboutMenu), chat.helpMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(chat.menubar), chat.fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(chat.menubar), chat.aboutMi);
  gtk_box_pack_start(GTK_BOX(chat.vbox), chat.menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(chat.vbox), chat.infoLabelHBox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(chat.infoLabelHBox), chat.connectionLbl, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(chat.vbox), chat.infoHBox, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(chat.infoHBox), chat.userEntry, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.infoHBox), chat.ipEntry, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.infoHBox), chat.portEntry, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.infoHBox), chat.sendInfoBtn, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.vbox), chat.hBox, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(chat.hBox), chat.vMainBox, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(chat.hBox), chat.vFriendsBox, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.vMainBox), chat.messagesScrollWindow, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(chat.vMainBox), chat.statusLbl, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(chat.vMainBox), chat.hInputBox, FALSE, FALSE, 10);

  gtk_box_pack_start(GTK_BOX(chat.vFriendsBox), chat.friendsScrollWindow, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(chat.hInputBox), chat.chatEntry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(chat.hInputBox), chat.sendBtn, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(chat.hInputBox), chat.statusCombo, TRUE, TRUE, 0);

  // render user list
  // sizeof(chat.users)/sizeof(chat.users[0])
  for (i = 0; i < chat.totalUsers; i++) {
    sprintf(chat.buffer, "User is: %s", chat.users[i].status);
    chat.msg = gtk_label_new(chat.buffer);
    gtk_misc_set_alignment(GTK_MISC(chat.msg), 0.0, 0.5);
    chat.friendInfoBtn = gtk_button_new_with_label("View info");
    chat.friendSendChatBtn = gtk_button_new_with_label(chat.users[i].name);
    chat.hFriendInfoBox = gtk_hbox_new(TRUE, 0); 

    g_signal_connect(GTK_OBJECT(chat.friendSendChatBtn), "clicked", GTK_SIGNAL_FUNC(renderMessages), &chat);   

    gtk_box_pack_start(GTK_BOX(chat.hFriendInfoBox), chat.friendSendChatBtn, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(chat.hFriendInfoBox), chat.friendInfoBtn, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(chat.hFriendInfoBox), chat.msg, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(chat.vFriendsBoxView), chat.hFriendInfoBox, FALSE, FALSE, 5);
  }  

  // render example
//  for (i = 0; i < 10; i++) {
//    for (j = 0; j < 10; j++){
//      sprintf(chat.buffer, "Message (%d,%d)\n", i, j);
//      chat.msg = gtk_label_new(chat.buffer);
//      gtk_box_pack_start(GTK_BOX(chat.vChatBox), chat.msg, FALSE, FALSE, 0);
//    }
//  }


  g_signal_connect(G_OBJECT(chat.window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(chat.quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(chat.helpMi), "activate", G_CALLBACK(showHelp), (gpointer) chat.window);

  gtk_widget_show_all(chat.window);

  gtk_main();

  close(sockfd);

  return 0;
}
