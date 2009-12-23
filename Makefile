all: simplest-player
simplest-player: simplest-player.c
	gcc -Wall -g `pkg-config --cflags --libs gstreamer-0.10` simplest-player.c -o simplest-player 
