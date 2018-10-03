#include <gtk/gtk.h>
#include <gmodule.h>
#include <string.h>

GtkWidget *v_chat_box;

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

typedef struct {
  GtkWidget *vChatBox;
  char *from;
} renderMessageSignal;

// Here we need to make the call to the server to fill up messages list
int renderMessages(GtkWidget *widget, GtkWidget *vChatBox){

  //GtkBox *vChatBox = (GtkBox*) data;

  //renderMessageSignal *rms = data;

  // array to hold messages
  message messages[2];
  GtkWidget *msg;
  char buffer[32];
  int i;

  //GtkWidget *vChatBox; 
  //char *from; 
  //vChatBox = rms->vChatBox;
  //from = rms->from;

  // temporary message creation for testing
  message msg1, msg2;
  msg1.from = "user_1";
  msg1.message = "msg_1";
  messages[0] = msg1;
  msg2.from = "user_2";
  msg2.message = "msg_2";
  messages[1] = msg2;

  for (i = 0; i < sizeof(messages)/sizeof(messages[0]); i++) {
    if (strcmp(gtk_button_get_label(GTK_BUTTON(widget)), messages[i].from) == 0) {
        //printf("rms %s == to %s\n", rms->from, messages[i].from);
        sprintf(buffer, "%s: %s\n", messages[i].from, messages[i].message);
        msg = gtk_label_new(buffer);
        gtk_misc_set_alignment(GTK_MISC(msg), 0.0, 0.5);
        gtk_box_pack_start(GTK_BOX(vChatBox), msg, FALSE, FALSE, 0);
        g_print("%s: %s\n", messages[i].from, messages[i].message);
    }    
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // GTK widgets (UI elements)
  GtkWidget *window, *vbox, *vMainBox, *vFriendsBox, *hInputBox, *menubar, *fileMenu, *aboutMenu, *fileMi, *quitMi, *aboutMi, *helpMi, *sendBtn, *chatEntry, *statusCombo, *hBox, *friendsLabel, *messagesScrollWindow, *vChatBox, *friendsScrollWindow, *vFriendsBoxView, *hFriendInfoBox, *friendInfoBtn, *friendSendChatBtn, *msg;

  char buffer[32];
  char currentFrom[32];
  int i, j;

  // array to hold users
  user users[2];

  // temporary users creation for testing
  user usr1, usr2;
  usr1.id = "0";
  usr1.name = "user_1";
  usr1.status = "active";
  users[0] = usr1;
  usr2.id = "1"; 
  usr2.name = "user_2";
  usr2.status = "busy";
  users[1] = usr2;

  gtk_init(&argc, &argv);

  // main window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_title(GTK_WINDOW(window), "Chactter");

  // scrollable window for messages
  messagesScrollWindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (messagesScrollWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  // scrollable window for users in chatroom
  friendsScrollWindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (friendsScrollWindow),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_widget_set_size_request(messagesScrollWindow, 300, 300);
  gtk_widget_set_size_request(friendsScrollWindow, 300, 300);
  
  // boxes
  vbox = gtk_vbox_new(FALSE, 0);
  vMainBox = gtk_vbox_new(FALSE, 0);
  vChatBox = gtk_vbox_new(FALSE, 0);
  //v_chat_box = gtk_vbox_new(FALSE, 0);
  vFriendsBox = gtk_vbox_new(FALSE, 0);
  vFriendsBoxView = gtk_vbox_new(FALSE, 0);  
  hInputBox = gtk_hbox_new(FALSE, 0);
  hBox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(messagesScrollWindow), vChatBox);

  //gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(messagesScrollWindow), v_chat_box);

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(friendsScrollWindow), vFriendsBoxView);

  // top menu elements
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  aboutMenu = gtk_menu_new();

  fileMi = gtk_menu_item_new_with_label("File");
  quitMi = gtk_menu_item_new_with_label("Quit");
  aboutMi = gtk_menu_item_new_with_label("About");
  helpMi = gtk_menu_item_new_with_label("Help");

  // GTK widgets
  sendBtn = gtk_button_new_with_label("Send");  

  chatEntry = gtk_entry_new();

  friendsLabel = gtk_label_new("Connected people");

  statusCombo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Active");
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Busy");
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Inactive");

  // widget placement
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(aboutMi), aboutMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(aboutMenu), helpMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hBox, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(hBox), vMainBox, FALSE, FALSE, 10);
  gtk_box_pack_start(GTK_BOX(hBox), vFriendsBox, TRUE, TRUE, 10);
  gtk_box_pack_start(GTK_BOX(vMainBox), messagesScrollWindow, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vMainBox), hInputBox, FALSE, FALSE, 10);

  //gtk_box_pack_start(GTK_BOX(vFriendsBox), friendsLabel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vFriendsBox), friendsScrollWindow, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(hInputBox), chatEntry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hInputBox), sendBtn, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hInputBox), statusCombo, TRUE, TRUE, 0);

  // render messages
  //renderMessages(vChatBox, "user");

  // render user list
  for (i = 0; i < sizeof(users)/sizeof(users[0]); i++) {
    sprintf(buffer, "%s: %s", users[i].name, users[i].status);
    msg = gtk_label_new(buffer);
    gtk_misc_set_alignment(GTK_MISC(msg), 0.0, 0.5);
    friendInfoBtn = gtk_button_new_with_label("View info");
    friendSendChatBtn = gtk_button_new_with_label(users[i].name);
    hFriendInfoBox = gtk_hbox_new(TRUE, 0); 

    //renderMessageSignal rms;
    //rms.vChatBox = vChatBox;
    //rms.from = users[i].name;

    //printf("In for loop\n");
    //printf("from is: %s\n", rms.from);

    g_signal_connect(G_OBJECT(friendSendChatBtn), "clicked", G_CALLBACK(renderMessages), G_OBJECT(vChatBox));   

    gtk_box_pack_start(GTK_BOX(hFriendInfoBox), msg, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(hFriendInfoBox), friendInfoBtn, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(hFriendInfoBox), friendSendChatBtn, TRUE, TRUE, 2);
    gtk_box_pack_start(GTK_BOX(vFriendsBoxView), hFriendInfoBox, FALSE, FALSE, 5);
  }  

  // render example
//  for (i = 0; i < 10; i++) {
//    for (j = 0; j < 10; j++){
//      sprintf(buffer, "Message (%d,%d)\n", i, j);
//      msg = gtk_label_new(buffer);
//      gtk_box_pack_start(GTK_BOX(v_chat_box), msg, FALSE, FALSE, 0);
//    }
//  }


  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
