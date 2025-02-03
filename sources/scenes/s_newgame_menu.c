#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../savesystem.h"
#include "../auth.h"
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
            // New game
            case 4:{
                if (getCurrentUser() == NULL){
                    Log("User not signed in", _DEBUG_);
                    break;
                }

                SaveInfo* saveinfo = (SaveInfo *) malloc(1 * sizeof(SaveInfo));
                InputFieldExtra* snameExtra = (InputFieldExtra *) menu.uiElements[0]->UIExtra;
                CarouselExtra* sizeExtra = (CarouselExtra *) menu.uiElements[1]->UIExtra;
                CarouselExtra* difficultyExtra = (CarouselExtra *) menu.uiElements[2]->UIExtra;
                CarouselExtra* playerSkin = (CarouselExtra *) menu.uiElements[3]->UIExtra;

                switch(playerSkin->curOption){
                case 0:
                    saveinfo->playerSkin = COLOR_MAROON;
                    break;
                case 1:
                    saveinfo->playerSkin = COLOR_CYAN;
                    break;
                case 2:
                    saveinfo->playerSkin = COLOR_PURPLE;
                    break;
                }

                switch(difficultyExtra->curOption){
                case 0:
                    saveinfo->difficulty.hungerDrainTicks = 10;
                    saveinfo->difficulty.healthRegenCDT = 1;
                    saveinfo->difficulty.maxArmor = 25;
                    saveinfo->difficulty.maxHealth = 40;
                    saveinfo->difficulty.maxHunger = 50;
                    saveinfo->difficulty.starveThreshold = 5;
                    break;
                case 1:
                    saveinfo->difficulty.hungerDrainTicks = 5;
                    saveinfo->difficulty.healthRegenCDT = 2;
                    saveinfo->difficulty.maxArmor = 10;
                    saveinfo->difficulty.maxHealth = 20;
                    saveinfo->difficulty.maxHunger = 25;
                    saveinfo->difficulty.starveThreshold = 10;
                    break;
                case 2:
                    saveinfo->difficulty.hungerDrainTicks = 2;
                    saveinfo->difficulty.healthRegenCDT = 3;
                    saveinfo->difficulty.maxArmor = 5;
                    saveinfo->difficulty.maxHealth = 10;
                    saveinfo->difficulty.maxHunger = 10;
                    saveinfo->difficulty.starveThreshold = 5;
                    break;
                case 3:
                    saveinfo->difficulty.hungerDrainTicks = 2;
                    saveinfo->difficulty.healthRegenCDT = 4;
                    saveinfo->difficulty.maxArmor = 1;
                    saveinfo->difficulty.maxHealth = 5;
                    saveinfo->difficulty.maxHunger = 5;
                    saveinfo->difficulty.starveThreshold = 3;
                    break;
                }

                strcpy(saveinfo->savename, snameExtra->input);
                createSave(saveinfo);

                loadGame(saveinfo->savename);
                if (!getCurrentSave()->gameFinished){
                    changeScene(getSceneByID("game"));
                }

                break;
            }
            // Back
            case 5:
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

void initscene_newgame_menu(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 7;
    menu.num_interactable_elements = 6;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 100}, "Save Name :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->maxLength = 20;
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createCarousel((Pos) {RWINDOW_WIDTH/2-150, 150}, "Map Size :",(char *[]){"Normal", "Large", "Gamer", "SigmaBoy"}, 4, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createCarousel((Pos) {RWINDOW_WIDTH/2-150, 200}, "Difficulty :",(char *[]){"Crybaby", "Skibidi", "Alpha", "Sigma"}, 4, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[3] = createCarousel((Pos) {RWINDOW_WIDTH/2-150, 250}, "Skin :",(char *[]){"Maroon", "Cyan", "Purple"}, 3, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[3], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[4] = createButton((Pos) {-1, 300}, "Create", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[4], COLOR_GRAY, COLOR_LIME_GREEN);

    menu.uiElements[5] = createButton((Pos) {-1, 350}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[5], COLOR_GRAY, COLOR_RUBY);

    menu.uiElements[6] = createLabel((Pos) {-1, 40}, "Configure your save", FONTNORMALSCALE*1.5, COLOR_BEIGE);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "newgame_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
