#include <gtk/gtk.h>

int main(int argc, char *argv[]) {

  GtkWidget *window, *vbox, *vMainBox, *vFriendsBox, *hInputBox, *menubar, *fileMenu, *aboutMenu, *fileMi, *quitMi, *aboutMi, *helpMi, *sendBtn, *chatEntry, *statusCombo, *hBox, *friendsLabel;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_title(GTK_WINDOW(window), "Chactter");

  vbox = gtk_vbox_new(FALSE, 0);
  vMainBox = gtk_vbox_new(FALSE, 0);
  vFriendsBox = gtk_vbox_new(FALSE, 0);
  hInputBox = gtk_hbox_new(FALSE, 0);
  hBox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  aboutMenu = gtk_menu_new();

  fileMi = gtk_menu_item_new_with_label("File");
  quitMi = gtk_menu_item_new_with_label("Quit");
  aboutMi = gtk_menu_item_new_with_label("About");
  helpMi = gtk_menu_item_new_with_label("Help");

  sendBtn = gtk_button_new_with_label("Send");

  chatEntry = gtk_entry_new();

  friendsLabel = gtk_label_new("Connected people");

  statusCombo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Active");
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Busy");
  gtk_combo_box_append_text(GTK_COMBO_BOX(statusCombo), "Inactive");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(aboutMi), aboutMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), quitMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(aboutMenu), helpMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMi);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hBox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hBox), vMainBox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hBox), vFriendsBox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vMainBox), hInputBox, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(hInputBox), chatEntry, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hInputBox), sendBtn, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hInputBox), statusCombo, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vFriendsBox), friendsLabel, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(quitMi), "activate",
        G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
