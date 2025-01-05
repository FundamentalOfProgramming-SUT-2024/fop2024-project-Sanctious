#include "scene.h"
#include "renderer.h"
#include "config.h"
#include "uiutils.h"

static Menu menu;

static void render() {

    glClear(GL_COLOR_BUFFER_BIT);
    renderMenu(&menu);
    glFlush();
}


static void processKeyboard(unsigned char key, int x, int y) {
    // Basic menu keyboard handling is done by UIutils
    menuBasicHandleKeyboard(&menu, key);

	// Manually handle buttons
	// key == 13 -> Enter key
    if (key == 13){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1)
        if (menu.uiElements[menu.hover_element]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu.uiElements[menu.hover_element]->UIExtra;
            // Manually handle button press
            switch(menu.hover_element){
            // Load games
            case 0:
                changeScene(getSceneByID("game"));
                break;

            }

        }
    }

}

static void processSKeyboard(int key, int x, int y) {
    // Basic menu keyboard handling is done by UIutils
    menuBasicHandleSKeyboard(&menu, key);
}

static void onExit(){
    menu.hover_element = -1;
}

void initscene_loadgame_menu(){
    // Menu
    menu.num_elements = 1;
    menu.num_interactable_elements = 1;
    menu.hover_element = -1;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 100}, "Game", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->maxLength = 20;
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);


    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "loadgame_menu");

    scene->onEnter = NULL;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
