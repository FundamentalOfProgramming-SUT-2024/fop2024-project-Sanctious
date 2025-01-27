#ifndef CONFIG_H
#define CONFIG_H

//  *********************
//  ****Game Settings****
//  *********************

// Window
#define WINDOW_WIDTH        800
#define WINDOW_HEIGHT       600
#define WINDOW_XPOS         500
#define WINDOW_YPOS         250
#define XBUFFER_ZONE        0
#define YBUFFER_ZONE        30
#define LOWER_YBUFFER_ZONE  30
#define EXTRA_BUFFER        40 // Extra space for walls
// Real sizes
#define RWINDOW_WIDTH       (WINDOW_WIDTH+XBUFFER_ZONE+EXTRA_BUFFER)
#define RWINDOW_HEIGHT      (WINDOW_HEIGHT+YBUFFER_ZONE+EXTRA_BUFFER+LOWER_YBUFFER_ZONE)


// Map
#define XCELLS              80
#define YCELLS              60
#define MAPDIV              3
#define MIN_ROOM_WIDTH      4
#define MIN_ROOM_HEIGHT     4
#define ROOM_TOPBUFFER      2 // Below 2 is buggy
#define ROOM_RIGHTBUFFER    2 // Below 2 is buggy
#define ROOM_LEFTBUFFER     2 // Below 2 is buggy
#define ROOM_BOTTOMBUFFER   2 // Below 2 is buggy

// Render
#define FONTQUALITY         96 // 24 * x
#define FONTNORMALSCALE     (0.25f) // 1 / x
#define CELLSIZE            (float) (WINDOW_WIDTH/XCELLS)

// Game Logic
#define MAX_FLOORS              5
#define MAX_INVENTORY_SIZE      16
#define MAX_NUM_SCENES          20
#define MAXITEMS                64
#define MAXDOORS                200
#define MAXROOMS                100
#define MAXSTRUCTURES           200
#define MAXCORRIDORS            100
#define MAX_CORRIDOR_LENGTH     50
#define MAX_STR_SIZE            256 // Codewise

// Credentials & UI
#define MAX_MENU_UIELEMENTS     40
#define MAX_SAVES_PU            5 // max saves per user
#define MAX_STRING_INPUT        100
#define MAX_CAROUSEL_OPTIONS    10
#define LINE_INDENTATION        5 // Pixel
#define INPUTBOX_TOPMARGIN      2 // Pixel
#define INPUTBOX_BOTTOMMARGIN   8 // Pixel
#define INPUTBOX_LEFTMARGIN     5 // Pixel
#define INPUTBOX_RIGHTMARGIN    5 // Pixel
#define PASSWORDMASK_CHAR       '*'
#define PASSWORD_MAXLENGTH      20
#define PASSWORD_MINLENGTH      7
#define POPUP_BOX_TOPMARGIN     10
#define POPUP_BOX_LEFTMARGIN    5
#define POPUP_BOX_RIGHTMARGIN   5
#define POPUP_BOX_BOTTOMMARGIN  10

#define COLOR_RED           (Color) {1.0f, 0.0f, 0.0f, 1.0f}
#define COLOR_GREEN         (Color) {0.0f, 1.0f, 0.0f, 1.0f}
#define COLOR_BLUE          (Color) {0.0f, 0.0f, 1.0f, 1.0f}
#define COLOR_WHITE         (Color) {1.0f, 1.0f, 1.0f, 1.0f}
#define COLOR_BLACK         (Color) {0.0f, 0.0f, 0.0f, 1.0f}
#define COLOR_YELLOW        (Color) {1.0f, 1.0f, 0.0f, 1.0f}
#define COLOR_CYAN          (Color) {0.0f, 1.0f, 1.0f, 1.0f}
#define COLOR_MAGENTA       (Color) {1.0f, 0.0f, 1.0f, 1.0f}
#define COLOR_GRAY          (Color) {0.5f, 0.5f, 0.5f, 1.0f}
#define COLOR_ORANGE        (Color) {1.0f, 0.5f, 0.0f, 1.0f}
#define COLOR_PURPLE        (Color) {0.5f, 0.0f, 0.5f, 1.0f}
#define COLOR_BROWN         (Color) {0.6f, 0.3f, 0.0f, 1.0f}
#define COLOR_CRIMSON       (Color) {0.86f, 0.08f, 0.24f, 1.0f}
#define COLOR_CORAL         (Color) {1.0f, 0.5f, 0.31f, 1.0f}
#define COLOR_TURQUOISE     (Color) {0.25f, 0.88f, 0.82f, 1.0f}
#define COLOR_TEAL          (Color) {0.0f, 0.5f, 0.5f, 1.0f}
#define COLOR_SALMON        (Color) {0.98f, 0.5f, 0.45f, 1.0f}
#define COLOR_LIME_GREEN    (Color) {0.2f, 0.8f, 0.2f, 1.0f}
#define COLOR_INDIGO        (Color) {0.29f, 0.0f, 0.51f, 1.0f}
#define COLOR_MAROON        (Color) {0.5f, 0.0f, 0.0f, 1.0f}
#define COLOR_GOLD          (Color) {1.0f, 0.84f, 0.0f, 1.0f}
#define COLOR_PEACH         (Color) {1.0f, 0.9f, 0.71f, 1.0f}
#define COLOR_LAVENDER      (Color) {0.9f, 0.9f, 0.98f, 1.0f}
#define COLOR_AQUAMARINE    (Color) {0.5f, 1.0f, 0.83f, 1.0f}
#define COLOR_SKY_BLUE      (Color) {0.53f, 0.81f, 0.92f, 1.0f}
#define COLOR_ROYAL_BLUE    (Color) {0.25f, 0.41f, 0.88f, 1.0f}
#define COLOR_BEIGE         (Color) {0.96f, 0.96f, 0.86f, 1.0f}
#define COLOR_IVORY         (Color) {1.0f, 1.0f, 0.94f, 1.0f}
#define COLOR_CHARTREUSE    (Color) {0.5f, 1.0f, 0.0f, 1.0f}
#define COLOR_PERIWINKLE    (Color) {0.8f, 0.8f, 1.0f, 1.0f}
#define COLOR_PLUM          (Color) {0.87f, 0.63f, 0.87f, 1.0f}
#define COLOR_SEA_GREEN     (Color) {0.18f, 0.55f, 0.34f, 1.0f}
#define COLOR_SIENNA        (Color) {0.63f, 0.32f, 0.18f, 1.0f}
#define COLOR_MINT          (Color) {0.74f, 0.99f, 0.79f, 1.0f}
#define COLOR_SLATE_GRAY    (Color) {0.44f, 0.5f, 0.56f, 1.0f}
#define COLOR_HOT_PINK      (Color) {1.0f, 0.41f, 0.71f, 1.0f}
#define COLOR_FUCHSIA       (Color) {1.0f, 0.0f, 1.0f, 1.0f}
#define COLOR_ELECTRIC_BLUE (Color) {0.49f, 0.98f, 1.0f, 1.0f}
#define COLOR_AMBER         (Color) {1.0f, 0.75f, 0.0f, 1.0f}
#define COLOR_EMERALD       (Color) {0.31f, 0.78f, 0.47f, 1.0f}
#define COLOR_RUBY          (Color) {0.88f, 0.07f, 0.37f, 1.0f}
#define COLOR_AMETHYST      (Color) {0.6f, 0.4f, 0.8f, 1.0f}
#define COLOR_TOPAZ         (Color) {0.98f, 0.84f, 0.65f, 1.0f}

void validateConfig();

#endif
