#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../player.h"
#include "../savesystem.h"

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
            // Back
            case 0:
                changeScene(getSceneByID("main_menu"));
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

}

static void onEnter(){
    Player* player = getPlayerInstance();
    SaveInfo* saveinfo = getCurrentSave();

    // Menu
    menu.num_elements = 3;
    menu.num_interactable_elements = 1;

    menu.uiElements[0] = createButton((Pos) {-1, 400}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[0], COLOR_GRAY, COLOR_RUBY);

    char temp[100];
    Color color = COLOR_CYAN;
    if (player->health == 0){
        sprintf(temp, "You lost");
        color = COLOR_RED;
    }
    else{
        sprintf(temp, "You won");
        color = COLOR_LIME_GREEN;
    }
    menu.uiElements[2] = createLabel((Pos) {-1, 200}, temp, FONTNORMALSCALE*2.5, color);

    sprintf(temp, "Golds: %d", player->gold);
    menu.uiElements[1] = createLabel((Pos) {-1, 300}, temp, FONTNORMALSCALE, COLOR_CYAN);

    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_endgame(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 3;
    menu.num_interactable_elements = 3;

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "endgame");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}

