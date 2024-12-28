#ifndef CONFIG_H
#define CONFIG_H

//  *********************
//  ****Game Settings****
//  *********************


// Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define WINDOW_XPOS 500
#define WINDOW_YPOS 250


// Map
#define XCELLS 40
#define YCELLS 40
#define MAPDIV 3
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 4
#define BUFFER_ZONE 1

// Render
#define CELLSIZE (float) (WINDOW_WIDTH/XCELLS)

// Game Logic

// its a bad practice but i have now other idea
#define MAXITEMS 64
#define MAXROOMS 10
#define MAXCORRIDORS
#define MAX_CORRIDOR_LENGTH 50


#endif
