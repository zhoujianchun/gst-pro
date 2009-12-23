#include <gst/gst.h>
static gboolean bus_cb(GstBus *bus, GstMessage *msg, gpointer data)
{
    GMainLoop *loop = data;
    switch(GST_MESSAGE_TYPE(msg))
    {
	case GST_MESSAGE_EOS:
	    g_print("playing finished\n");
	    g_main_loop_quit(loop);
	    break;
	case GST_MESSAGE_ERROR:
	    g_main_loop_quit(loop);
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

    play = gst_element_factory_make("playbin2", "play");
	GstElement* audio_sink = gst_element_factory_make ("lpesink", "audio-sink");
	g_object_set(G_OBJECT(play), "audio-sink", audio_sink, NULL); 
	//play->set_properties("volume", 10);
    loop = g_main_loop_new(NULL, FALSE);

    g_object_set(G_OBJECT(play), "uri", argv[1], NULL);
    gst_bus_add_watch(gst_pipeline_get_bus(GST_PIPELINE(play)), bus_cb, loop);
    g_print("playing......\n");
    gst_element_set_state(play, GST_STATE_PLAYING);
    g_print("start g_main_loop_run\n");
    g_main_loop_run(loop);
    g_print("g_main_loop_run return\n");
    gst_element_set_state(play, GST_STATE_NULL);
    return 0;
}
