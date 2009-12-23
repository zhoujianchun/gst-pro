#include <gst/gst.h>
int main(int argc, chara **argv)
{
    GstElement *pipeline;
    GstElement *source, *filter, *sink;

    gst_init(&argc, &argv);
    pipeline = gst_pipeline_new("my-pipeline");

    source = gst_element_factory_make("fakesrc", "source");
    filter = gst_element_factory_make("identity", "filter");
    sink = gst_element_factory_make("fakesink", "sink");

    gst_bin_add_many(GST_BIN(pipeline), source, filter, sink, NULL);

    if(!gst_element_link_many(source, filter, sink, NULL))
    {
	g_warning("Failed to link elements!");
    }
}
