#include <gtk/gtk.h>
#include "base64.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <mio.hpp>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
struct messageBuf
{
    long msg_type;
    char mtext[256];
};
int handle_error(const std::error_code& error)
{
    const auto& errmsg = error.message();
    std::printf("error mapping file: %s, exiting...\n", errmsg.c_str());
    return error.value();
}

void allocate_file(const std::string& path, const int size)
{
    std::ofstream file(path);
    std::string s(size, '0');
    file << s;
}
static void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);    

    std::string mmppath = "/home/jsh/mmp";

    struct messageBuf mybuf;

    allocate_file(mmppath, 50000);

    std::error_code error;
    mio::ummap_sink rw_mmap;
    rw_mmap.map(mmppath, 0, mio::map_entire_file, error);

    key_t mqKey = msgget(115200,IPC_CREAT|0666);
    if (mqKey == -1)
    {
        perror("msgget error : ");
        exit(0);
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    GtkWidget *image = gtk_image_new_from_file("/home/jsh/overwatch.jpeg");
    // GIcon gicon ;
    // gtk_image_get_gicon(image,)
    // gtk_window_resize(GTK_WINDOW(window),1440,2560);

    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);


    // if (msgrcv( mqKey, &mybuf, sizeof(messageBuf) - sizeof(long), 1, 0) == -1){
        // perror( "msgrcv() 실패");
        // exit(1);
    // } 

    gtk_main();

    // std::vector<uint8_t> buff(0);

    return 0;
}