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
            // Register
            case 0:
                changeScene(getSceneByID("registeruser_menu"));
                break;
            // Login
            case 1:
                changeScene(getSceneByID("loginuser_menu"));
                break;

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
    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_authentication_menu(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 4;
    menu.num_interactable_elements = 3;

    menu.uiElements[0] = createButton((Pos) {-1, 200}, "Register", FONTNORMALSCALE*1.5);
    configureButtonColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createButton((Pos) {-1, 320}, "Login", FONTNORMALSCALE*1.5);
    configureButtonColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createButton((Pos) {-1, 420}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[2], COLOR_GRAY, COLOR_RUBY);

    menu.uiElements[3] = createLabel((Pos) {-1, 80}, "Authentication", FONTNORMALSCALE*2, COLOR_EMERALD);
    // configureButtonColor(menu.uiElements[3], COLOR_GRAY, COLOR_RUBY);

//    menu.uiElements[4] = createInputField((Pos) {-1, 300}, "", FONTNORMALSCALE, (Scale) {300, 30}, 20);
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->masking = 1;
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->maxLength = PASSWORD_MAXLENGTH;
//    menu.uiElements[5] = createInputField((Pos) {-1, 350}, "Register :", FONTNORMALSCALE,(Scale) {300, 30}, 20);
//    menu.uiElements[6] = createCarousel((Pos) {-1, 400}, "Options :",(char *[]){"Hello", "Test", "Poopak"}, 3, FONTNORMALSCALE);
//    menu.uiElements[7] = createSlider((Pos) {-1, 450}, "Slider :", 50, 0, 100, 5, FONTNORMALSCALE, 50);
//    menu.uiElements[7] = createLabel((Pos) {-1, 460}, "Enter", FONTNORMALSCALE, COLOR_CRIMSON);
//    menu.uiElements[7] = createLabel((Pos) {-1, 40}, "Welcome to Roþue!", FONTNORMALSCALE*2, COLOR_CRIMSON);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "authentication_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}

