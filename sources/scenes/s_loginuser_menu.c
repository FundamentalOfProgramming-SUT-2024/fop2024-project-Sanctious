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
            // Login
            case 2:

                resetMsgPopUp(&menu);
                char* name = ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->input;
                char* password = ((InputFieldExtra *) menu.uiElements[1]->UIExtra)->input;

                User* user = loadUser(name, password);

                if (user == NULL){
                    addMsgToPopUp(&menu, "User/Password incorrect.");
                }
                else{
                    setCurrentUser(user);
                    changeScene(getSceneByID("main_menu"));
                }

                activatePopUp(&menu, COLOR_RUBY);
                break;
            // Quit
            case 3:
                changeScene(getSceneByID("authentication_menu"));
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
    resetMenuFields(&menu);
    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_login_menu(){
    // Menu
    menu.num_elements = 5;
    menu.num_interactable_elements = 4;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 100}, "Name :", FONTNORMALSCALE, (Scale) {150, 30}, 20);
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);
    ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->maxLength = 30;

    menu.uiElements[1] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 150}, "Password :", FONTNORMALSCALE, (Scale) {150, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[1]->UIExtra)->masking = 1;
    ((InputFieldExtra *) menu.uiElements[1]->UIExtra)->maxLength = PASSWORD_MAXLENGTH;
    configureInputFieldColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createButton((Pos) {-1, 300}, "Login", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[3] = createButton((Pos) {-1, 350}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[3], COLOR_GRAY, COLOR_LIME_GREEN);

    menu.uiElements[4] = createLabel((Pos) {-1, 40}, "Login sigmaboi!", FONTNORMALSCALE*2, COLOR_CRIMSON);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "loginuser_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
