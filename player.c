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
int main(int argc, char** argv)
{
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
	gtk_widget_set_size_request(window, 400, 400);

    play = gst_element_factory_make("playbin2", "play");
	char* str = g_strdup_printf("file://%s", argv[1]);
    g_object_set(G_OBJECT(play), "uri", str, NULL);
	g_free(str);
	gst_bus_set_sync_handler (gst_pipeline_get_bus(GST_PIPELINE(play)), (GstBusSyncHandler) create_window, play);
    gst_element_set_state(play, GST_STATE_PLAYING);

	gtk_widget_show_all(window);

	gtk_main();
    return 0;
}
