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


// Map
#define XCELLS 40
#define YCELLS 30
#define MAPDIV 3
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 4
#define BUFFER_ZONE 1

// Render
#define CELLSIZE (float) (WINDOW_WIDTH/XCELLS)

// Game Logic

// its a bad practice but i have now other idea
#define MAXITEMS 64
#define MAXDOORS 10
#define MAXROOMS 10
#define MAXCORRIDORS
#define MAX_CORRIDOR_LENGTH 50


#endif
