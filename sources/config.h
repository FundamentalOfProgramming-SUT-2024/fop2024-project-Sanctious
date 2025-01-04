#ifndef CONFIG_H
#define CONFIG_H

//  *********************
//  ****Game Settings****
//  *********************

// Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_XPOS 500
#define WINDOW_YPOS 250
#define XBUFFER_ZONE 0
#define YBUFFER_ZONE 30
#define LOWER_YBUFFER_ZONE 30
#define EXTRA_BUFFER 40 // Extra space for walls
// Real sizes
#define RWINDOW_WIDTH (WINDOW_WIDTH+XBUFFER_ZONE+EXTRA_BUFFER)
#define RWINDOW_HEIGHT (WINDOW_HEIGHT+YBUFFER_ZONE+EXTRA_BUFFER+LOWER_YBUFFER_ZONE)

#define MAX_SCENES 10


// Map
#define XCELLS 40
#define YCELLS 30
#define MAPDIV 3
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 4
#define BUFFER_ZONE 1

// Render
#define FONTQUALITY 96 // 24 * x
#define FONTNORMALSCALE (0.25f) // 1 / x
#define CELLSIZE (float) (WINDOW_WIDTH/XCELLS)

// Game Logic

// its a bad practice but i have now other idea
#define MAX_NUM_SCENES 10
#define MAXITEMS 64
#define MAXDOORS 100
#define MAXROOMS 100
#define MAXCORRIDORS 50
#define MAX_CORRIDOR_LENGTH 50
#define MAX_STR_SIZE 256
#define MAX_UIELEMENTS 40


#endif
