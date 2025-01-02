#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);
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

    scene->sceneID = (char *) malloc(MAX_STR_SIZE * sizeof(char));
    strcpy(scene->sceneID, "main_menu");

    scene->onEnter = NULL;
    scene->onExit = NULL;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
