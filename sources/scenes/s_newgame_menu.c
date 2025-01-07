#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"

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
            case 4:

                break;
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
    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_newgame_menu(){
    // Menu
    menu.num_elements = 7;
    menu.num_interactable_elements = 6;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 100}, "Save Name :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->maxLength = 20;
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createCarousel((Pos) {RWINDOW_WIDTH/2-150, 150}, "Map Size :",(char *[]){"Normal", "Large", "Gamer", "SigmaBoy"}, 4, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createCarousel((Pos) {RWINDOW_WIDTH/2-150, 200}, "Difficulty :",(char *[]){"Loser", "Mid", "Nerd", "Gamer"}, 4, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[3] = createSlider((Pos) {RWINDOW_WIDTH/2-150, 250}, "Survival Chance :", 50, 0, 100, 5, FONTNORMALSCALE, 50);
    configureSliderColor(menu.uiElements[3], COLOR_GRAY, COLOR_CYAN);

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
