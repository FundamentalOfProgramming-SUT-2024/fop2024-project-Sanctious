#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../auth.h"
#include "../savesystem.h"
#include "../logger.h"

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
            case 1:
                resetMsgPopUp(&menu);
                if (getCurrentUser() == NULL){
                    Log("User not signed in", _DEBUG_);
                    break;
                }

                InputFieldExtra* extra = (InputFieldExtra *) menu.uiElements[0]->UIExtra;

                if (!saveExists(extra->input)){
                    addMsgToPopUp(&menu, "This save file doesn't exist!");
                    activatePopUp(&menu, COLOR_RUBY);
                    break;
                }

                loadGame(extra->input);
                if (!getCurrentSave()->gameFinished){
                    changeScene(getSceneByID("game"));
                }
                else{
                    addMsgToPopUp(&menu, "This save is already finished!");
                }


                activatePopUp(&menu, COLOR_RUBY);
                break;
            // Back
            case 2:
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
    setCurrentUser(loadUser("test", NULL));

    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_loadgame_menu(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 3;
    menu.num_interactable_elements = 3;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 100}, "Game", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->maxLength = 20;
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createButton((Pos) {-1, 150}, "Load", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[1], COLOR_GRAY, COLOR_LIME_GREEN);

    menu.uiElements[2] = createButton((Pos) {-1, 200}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[2], COLOR_GRAY, COLOR_RUBY);


    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "loadgame_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
