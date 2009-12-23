#include <glib.h>
#include <stdlib.h>
#include <stdio.h>

int i = 0;
gboolean timeout_one_cb(gpointer data)
{
	printf("enter one\n");
	i++;
	printf("%d\n", i);
	return TRUE;
}

gboolean timeout_two_cb(gpointer data)
{
	printf("enter two\n");
	i++;
	printf("%d\n", i);
	return TRUE;
}

gint main(gint argc, gchar **argv)
{
	g_timeout_add_seconds(2, timeout_one_cb, NULL);
	g_timeout_add_seconds(2, timeout_two_cb, NULL);
	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);
	return 0;
}
