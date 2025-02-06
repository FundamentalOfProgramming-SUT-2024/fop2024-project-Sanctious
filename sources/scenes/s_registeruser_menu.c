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
            // Register
            case 3:
                resetMsgPopUp(&menu);
                char* name = ((InputFieldExtra *) menu.uiElements[0]->UIExtra)->input;
                char* email = ((InputFieldExtra *) menu.uiElements[1]->UIExtra)->input;
                char* password = ((InputFieldExtra *) menu.uiElements[2]->UIExtra)->input;

                int flag = 0;

                if (strlen(name) == 0){
                    addMsgToPopUp(&menu, "Username can't be empty.\n");
                    flag = 1;
                }

                if (userExists(name)){
                    addMsgToPopUp(&menu, "Username unavailable.\n");
                    flag = 1;
                }

                if (!isValidPassword(password)){
                    addMsgToPopUp(&menu, "Invalid password format.\n");
                    flag = 1;
                }

                if (!isValidEmail(email)){
                    addMsgToPopUp(&menu, "Invalid email format.\n");
                    flag = 1;
                }

                if (!flag){
                    // Create user
                    free(getCurrentUser());
                    setCurrentUser(createUser(name, email, password));
                    changeScene(getSceneByID("main_menu"));



                    // Add user to file system
                }
                else{
                    activatePopUp(&menu, COLOR_RUBY);
                }

                break;
            // Random password
            case 4:{
                resetMsgPopUp(&menu);
                char* password = ((InputFieldExtra *) menu.uiElements[2]->UIExtra)->input;
                char temp[100];
                generateRandomPassword(temp);
                strcpy(password, temp);
                sprintf(temp, "You'r password is %s", password);
                addMsgToPopUp(&menu, temp);
                activatePopUp(&menu, COLOR_LIME_GREEN);
                break;
            }
            // Back
            case 5:
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
    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
    resetMenuFields(&menu);
}

void initscene_register_menu(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 7;
    menu.num_interactable_elements = 6;

    menu.uiElements[0] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 150}, "Name :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    configureInputFieldColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 220}, "Email :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[1]->UIExtra)->maxLength = 30;
    configureInputFieldColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createInputField((Pos) {RWINDOW_WIDTH/2-150, 290}, "Password :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[2]->UIExtra)->masking = 1;
    ((InputFieldExtra *) menu.uiElements[2]->UIExtra)->maxLength = PASSWORD_MAXLENGTH;
    configureInputFieldColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[3] = createButton((Pos) {-1, 360}, "Register", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[3], COLOR_GRAY, COLOR_LIME_GREEN);

    menu.uiElements[4] = createButton((Pos) {-1, 430}, "Random Password", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[4], COLOR_GRAY, COLOR_LIME_GREEN);

    menu.uiElements[5] = createButton((Pos) {-1, 500}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[5], COLOR_GRAY, COLOR_RUBY);

    menu.uiElements[6] = createLabel((Pos) {-1, 70}, "Register", FONTNORMALSCALE*1.5, COLOR_EMERALD);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "registeruser_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
