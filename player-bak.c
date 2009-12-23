#include <gst/gst.h>
#include <gtk/gtk.h>
static gboolean bus_cb(GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = data;
    switch(GST_MESSAGE_TYPE(msg))
    {
	case GST_MESSAGE_EOS:
	    g_print("playing finished\n");
	    gtk_main_quit();
	    break;
	case GST_MESSAGE_ERROR:
	    gtk_main_quit();
	    break;
	default:
	    break;
    }
    return TRUE;
}
int main(int argc, char** argv)
{
    GMainLoop *loop;
    GstElement *play;

    gst_init(&argc, &argv);
	gtk_init(&argc, &argv);
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 200, 200);

    play = gst_element_factory_make("playbin2", "play");
	//play->set_properties("volume", 10);
    loop = g_main_loop_new(NULL, FALSE);

    g_object_set(G_OBJECT(play), "uri", argv[1], NULL);
    gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(play)), bus_cb, loop);
    g_print("playing......\n");
    gst_element_set_state(play, GST_STATE_PLAYING);
    g_print("start g_main_loop_run\n");
    g_print("g_main_loop_run return\n");
    gst_element_set_state(play, GST_STATE_NULL);
	gtk_widget_show_all(window);

	gtk_main();
    return 0;
}
