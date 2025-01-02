#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"

//widgetmanager->crearte color list of colors
//widgetManager ->getcolor by name

typedef struct{
    float r, g, b, a;
} Color;

//create a list of colors const

typedef struct{
    int x, y;
} Pos;


typedef struct{
    Pos pos;
    char label[MAX_STR_SIZE];
    int* scale;
    // A -> active, DA -> deactive
    Color* Acolor; // memory efficiency
    Color* Abgcolor; // memory efficiency
    Color* DAcolor; // memory efficiency
    Color* DAbgcolor; // memory efficiency

    int isActive;
} Button;


//
//menu->buttons
//menu->num buttons

//label

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Button* buttons[1];
    Button* btn = (Button *) malloc(1 * sizeof(Button));
    btn->pos = (Pos) {0, 50};

    strcpy(btn->label, "test");

    Color* red = (Color *) malloc(1 * sizeof(Color));
    *red = (Color) {0.5, 0.5, 0.5, 1};
    Color* blue = (Color *) malloc(1 * sizeof(Color));
    *blue = (Color) {0.5, 0.5, 0.5, 1};

    btn->Acolor = red;

    for (int i = 0; i < 1; i++){
        renderString(btn->pos.x, btn->pos.y, btn->label, btn->Acolor->r, btn->Acolor->g, btn->Acolor->b, btn->Acolor->a);
    }


    renderString(0, 20, "Welcome to rogue!", 0.5f, 0.1f, 0.9f, 1.0f);

    glFlush();
}

static void processSKeyboard(int key, int x, int y) {

	if (key == GLUT_KEY_F2){
        changeScene(getSceneByID("game"));
	}
}


void initscene_main_menu(){
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "main_menu");

    scene->onEnter = NULL;
    scene->onExit = NULL;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
