#include <gtk/gtk.h>
#include "base64.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include <thread>
#include <filesystem>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <mio.hpp>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <rapidjson/document.h>
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
struct messageBuf
{
    long msg_type;
    char mtext[256];
};

void ipc_thread(GtkWidget *image);
int handle_error(const std::error_code& error);
void allocate_file(const std::string& path);
static void destroy(GtkWidget *widget, gpointer data);

void cb(){
    std::cout << "DRAWWWWWWWWWWWw" << std::endl;
}
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);    

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    // gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

    gtk_window_fullscreen(GTK_WINDOW(window));

    gtk_widget_show_all(window);


    GtkWidget *image = gtk_image_new ();
    g_signal_connect(window, "draw", G_CALLBACK(cb), NULL);

    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);
    
    gtk_widget_set_size_request (image,workarea.width, workarea.height);

    std::cout << workarea.width<<workarea.height << std::endl;

    std::thread th(ipc_thread,image);

    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void ipc_thread(GtkWidget *image){

    std::string mmppath = "/opt/capsuleFW/print/mmap";

    struct messageBuf mybuf;

    allocate_file(mmppath);

    std::error_code error;

    mio::ummap_sink rw_mmap;
    rw_mmap.map(mmppath, 0, mio::map_entire_file, error);

    key_t mqKey = msgget(115200,IPC_CREAT|0666);
    if (mqKey == -1)
    {
        perror("msgget error : ");
        exit(0);
    }

    while(1){
        if (msgrcv( mqKey, &mybuf, sizeof(messageBuf), 1, 0) == -1){
            perror( "msgrcv() 실패");
            continue;
        }
        rapidjson::Document d;
        d.Parse(mybuf.mtext);

        rapidjson::Value& cmd = d["cmd"];

        if(!std::string(cmd.GetString(),cmd.GetStringLength()).compare("imageChanged")){
            
            rapidjson::Value& sizeV = d["size"];

            std::cout << "image change " << mybuf.mtext << std::endl;

            guint width, height;
            width = gtk_widget_get_allocated_width (image);
            height = gtk_widget_get_allocated_height (image);

            GdkPixbufLoader *loader = gdk_pixbuf_loader_new ();
            gdk_pixbuf_loader_write (loader,rw_mmap.begin(), sizeV.GetInt(), NULL);
            GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf (loader);
            // pixbuf=gdk_pixbuf_scale_simple(pixbuf,width,height,GDK_INTERP_BILINEAR);

            gdk_pixbuf_loader_close(loader,NULL);
            g_object_unref(pixbuf);

            gtk_image_set_from_pixbuf(GTK_IMAGE(image),pixbuf);

            gtk_widget_queue_draw(image);

        }
        
    }

}


void ipc_thread(GtkWidget *window);
int handle_error(const std::error_code& error)
{
    const auto& errmsg = error.message();
    std::printf("error mapping file: %s, exiting...\n", errmsg.c_str());
    return error.value();
}

void allocate_file(const std::string& path)
{
    std::filesystem::path fsPath(path);
    if(!std::filesystem::exists(fsPath)){
        std::ofstream out(path);
        std::filesystem::resize_file(fsPath,100000);
    }

}

static void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}
