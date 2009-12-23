#include <gst/gst.h>
#include <glib.h>
int main(int argc, char** argv)
{
    GstElement *pipeline, *filesrc, *decoder, *audiosink;
    gst_init(&argc, &argv);
    if(argc != 2)
    {
	g_print("usage: %s <mp3 filename>\n", argv[0]);
	exit(-1);
    }
    pipeline = gst_pipeline_new("pipeline");

    filesrc = gst_element_factory_make("filesrc", "disk_source");

    g_object_set(G_OBJECT(filesrc), "location", argv[1], NULL);

    decoder = gst_element_factory_make("mad", "decoder");

    audiosink = gst_element_factory_make("ossink", "play_audio");

    g_print("ossink pass\n");

    gst_bin_add_many(GST_BIN(pipeline), filesrc, decoder, audiosink, NULL);

    gst_element_link_many(filesrc, decoder, audiosink, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    while(gst_bin_iterate_elements(GST_BIN(pipeline)));

    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(GST_OBJECT(pipeline));

    exit(0);
}
