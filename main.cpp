#include <gtk/gtk.h>
#include "base64.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include <thread>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include  "communicate.h"

void ipc_thread(GtkWidget *image);
int handle_error(const std::error_code& error);
void allocate_file(const std::string& path);
static void destroy(GtkWidget *widget, gpointer data);


int size = 0;

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    // std::cout << Communicate::getInstance()._addr << size << std::endl;

    if(size == 0)
        return FALSE;

    int s = size;
    size = 0;

    if(Communicate::getInstance()._addr == NULL)
        return FALSE;

    std::cout << "HELLO FUCKING WORLD"<< std::endl;

    GdkPixbufLoader *loader = gdk_pixbuf_loader_new();

    gdk_pixbuf_loader_write (loader,Communicate::getInstance()._addr, s, NULL);
    
    GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);

    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);

    gdk_pixbuf_loader_close(loader,NULL);
    g_object_unref(pixbuf);

    return FALSE;
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

    GtkWidget *canvas = gtk_drawing_area_new ();
    g_signal_connect(canvas, "draw", G_CALLBACK(draw_callback), NULL);

    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
        gdk_display_get_primary_monitor(gdk_display_get_default()),
        &workarea);
    
    gtk_widget_set_size_request(canvas,workarea.width, workarea.height);
    gtk_container_add(GTK_CONTAINER(window), canvas);

    GdkColor color;
    color.red = 0x00ff;
    color.green = 0x00ff;
    color.blue = 0x00ff;

    gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);

    std::cout << workarea.width<<workarea.height << std::endl;

    std::thread th(ipc_thread,canvas);

    gtk_window_set_accept_focus(GTK_WINDOW(window),FALSE);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void ipc_thread(GtkWidget *canvas){


    std::error_code error;

    GdkPixbufLoader *loader = NULL;

    Communicate::getInstance();

    while ((1))
    {
        messageBuf mybuf;
        if (!Communicate::getInstance().receiveMessage(&mybuf,false)){
            std::cout << "msg clear" << std::endl;
            break;
        }
    }
    
    while(1){
        messageBuf mybuf;

        if (!Communicate::getInstance().receiveMessage(&mybuf,true)){
            perror( "msgrcv() 실패");
            exit(0);
        }
        std::cout << mybuf.mtext << std::endl;

        rapidjson::Document d;
        d.Parse(mybuf.mtext);

        rapidjson::Value& cmd = d["cmd"];

        if(!std::string(cmd.GetString(),cmd.GetStringLength()).compare("imageChanged")){
            
            rapidjson::Value& sizeV = d["size"];
            size = sizeV.GetInt();

            std::cout << "image change " << mybuf.mtext << std::endl;

            gtk_widget_queue_draw(canvas);
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
static void destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}
