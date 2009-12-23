#include <gst/gst.h>
#include <gtk/gtk.h>
#include <gst/interfaces/xoverlay.h>
#include <gdk/gdkx.h>

GstElement *play;
GtkWidget *draw_area;
GstElement *video_sink;

static GstBusSyncReply
create_window (GstBus * bus, GstMessage * message, GstPipeline * pipeline)
{
	 if (GST_MESSAGE_TYPE (message) != GST_MESSAGE_ELEMENT)
		    return GST_BUS_PASS;
	  
	  if (!gst_structure_has_name (message->structure, "prepare-xwindow-id"))
		     return GST_BUS_PASS;
		 
		    
		   gst_x_overlay_set_xwindow_id (GST_X_OVERLAY (GST_MESSAGE_SRC (message)),
				        GDK_WINDOW_XID(draw_area->window));
		     
		    gst_message_unref (message);
			  
			 return GST_BUS_DROP;
}

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
gboolean draw_area_expose_cb(gpointer data)
{
	GstXOverlay *overlay = GST_X_OVERLAY(video_sink);
	if(GST_IS_X_OVERLAY(overlay))
	{
		printf("overlay is overlay\n");
	}
	//GstXOverlay *overlay = GST_X_OVERLAY (gst_bin_get_by_interface (GST_BIN (video_sink), GST_TYPE_X_OVERLAY));

	gst_x_overlay_set_xwindow_id(overlay, GDK_WINDOW_XID(draw_area->window));

	return FALSE;
}
gboolean video_changed_cb(gpointer data)
{
	printf("video_changed_cb\n");
	GstXOverlay *overlay = GST_X_OVERLAY (gst_bin_get_by_interface (GST_BIN (play),
				                                           GST_TYPE_X_OVERLAY));
	

	gst_x_overlay_set_xwindow_id(overlay, GDK_WINDOW_XID(draw_area->window));
	return TRUE;
}
int main(int argc, char** argv)
{
    GMainLoop *loop;

    gst_init(&argc, &argv);
	gtk_init(&argc, &argv);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	draw_area = gtk_drawing_area_new();

    GtkWidget *box = gtk_vbox_new(FALSE, 0);
	
    GtkWidget *button = gtk_button_new_with_label("HELLO");
	gtk_box_pack_start(GTK_BOX(box), draw_area, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

	gtk_container_add(GTK_CONTAINER(window), box);
	gtk_drawing_area_size(GTK_DRAWING_AREA(draw_area), 200, 200);
	g_signal_connect (draw_area, "expose-event", G_CALLBACK (draw_area_expose_cb), NULL);
	gtk_widget_set_size_request(window, 400, 400);

    play = gst_element_factory_make("playbin2", "play");
	//g_signal_connect (play, "video-changed", G_CALLBACK (video_changed_cb), NULL);
    ///video_sink = gst_element_factory_make("MixVideoSink", "sink");
    video_sink = gst_element_factory_make("xvimagesink", "sink");
	if(!video_sink)
	{
		//printf("create video_sink failed\n");
		//exit(-1);
	}
    //video_sink = gst_element_factory_make("xvimagesink", "sink");
    loop = g_main_loop_new(NULL, FALSE);

    g_object_set(G_OBJECT(play), "uri", argv[1], NULL);
    //g_object_set(G_OBJECT(play), "video_sink", video_sink, NULL);
    gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(play)), bus_cb, loop);
	gst_bus_set_sync_handler (gst_pipeline_get_bus(GST_PIPELINE(play)), (GstBusSyncHandler) create_window, play);
    gst_element_set_state(play, GST_STATE_PLAYING);

	gtk_widget_show_all(window);
    g_object_set(G_OBJECT(play), "video_sink", video_sink, NULL);
	//GstXOverlay *overlay = GST_X_OVERLAY (gst_bin_get_by_interface (GST_BIN (play),
	//			                                           GST_TYPE_X_OVERLAY));
	//gst_x_overlay_set_xwindow_id(overlay, GDK_WINDOW_XID(draw_area->window));

	gtk_main();
    return 0;
}
