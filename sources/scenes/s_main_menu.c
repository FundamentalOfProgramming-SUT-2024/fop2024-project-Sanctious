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
            // Authentication
            case 0:
                changeScene(getSceneByID("authentication_menu"));
                break;
            // New game
            case 1:
                changeScene(getSceneByID("newgame_menu"));
                break;
            // Load game
            case 2:
                changeScene(getSceneByID("loadgame_menu"));
                break;
            // Profile
            case 3:
                changeScene(getSceneByID("profile_menu"));
                break;
            // Leaderboard
            case 4:
                changeScene(getSceneByID("leaderboard_menu"));
                break;
            // Settings
            case 5:
                changeScene(getSceneByID("settings_menu"));
                break;
            // Quit
            case 6:
                exit(0);
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

void initscene_main_menu(){
    // Menu
    menu.num_elements = 8;
    menu.num_interactable_elements = 7;
    menu.hover_element = -1;

    menu.uiElements[0] = createButton((Pos) {-1, 100}, "Authentication", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createButton((Pos) {-1, 150}, "New Game", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createButton((Pos) {-1, 200}, "Load Game", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[3] = createButton((Pos) {-1, 250}, "Profile", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[3], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[4] = createButton((Pos) {-1, 300}, "Leaderboard", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[4], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[5] = createButton((Pos) {-1, 350}, "Settings", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[5], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[6] = createButton((Pos) {-1, 410}, "Quit", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[6], COLOR_GRAY, COLOR_RUBY);

    menu.uiElements[7] = createLabel((Pos) {-1, 40}, "Welcome to Roþue!", FONTNORMALSCALE*2, COLOR_CRIMSON);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "main_menu");

    scene->onEnter = NULL;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
