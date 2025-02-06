#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../auth.h"

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
            // Authentication
            case 0:
                changeScene(getSceneByID("main_menu"));
                break;
            case 1:

                break;
            case 2:

                break;
            case 3:

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
    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);

    User* user = getCurrentUser();
    char temp[100];

    sprintf(temp, "Total gold: %d", user->stats.sumGold);
    strcpy(((LabelExtra *) menu.uiElements[1]->UIExtra)->label, temp);

    sprintf(temp, "Finished games: %d", user->stats.num_games);
    strcpy(((LabelExtra *) menu.uiElements[2]->UIExtra)->label, temp);

    sprintf(temp, "Total exp: %d", user->stats.exp);
    strcpy(((LabelExtra *) menu.uiElements[3]->UIExtra)->label, temp);

    sprintf(temp, "Playtime: %d", user->stats.playTime);
    strcpy(((LabelExtra *) menu.uiElements[4]->UIExtra)->label, temp);
}

void initscene_profile_menu(){
    // Menu'
    menu.enabled = 1;
    menu.num_elements = 5;
    menu.num_interactable_elements = 1;

    menu.uiElements[0] = createButton((Pos) {-1, 380}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[0], COLOR_GRAY, COLOR_RUBY);

    menu.uiElements[1] = createLabel((Pos) {500, 100}, "Total gold: yyyy", FONTNORMALSCALE, COLOR_CORAL);

    menu.uiElements[2] = createLabel((Pos) {500, 170}, "Finished games: yyyy", FONTNORMALSCALE, COLOR_CORAL);

    menu.uiElements[3] = createLabel((Pos) {500, 240}, "Total exp: yyyy", FONTNORMALSCALE, COLOR_CORAL);

    menu.uiElements[4] = createLabel((Pos) {500, 310}, "Playtime: yyyy", FONTNORMALSCALE, COLOR_CORAL);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "profile_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
