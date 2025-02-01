#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../auth.h"

static Menu menu;
static User* users[MAX_NUM_USERS];
static int count = 0;

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
    for (int i = 0; i < count; i++){
        free(users[i]);
    }
}

static int sort(const void* user1, const void* user2){
    User* usera = *(User **) user1;
    User* userb = *(User **) user2;
    long long diff = userb->stats.exp - usera->stats.exp;
    return (diff > 0) - (diff < 0);
}

static void onEnter(){
    count = loadAllUsers(users);
    qsort(users, count, sizeof(User *), sort);

    menu.num_elements = count;
    menu.num_interactable_elements = 0;

    char temp[100];
    for (int i = 0; i < count; i ++){
        sprintf(temp, "(%d). %s: g:%d pt:%d score:%d games:%d", i+1, users[i]->creds.name, users[i]->stats.sumGold, users[i]->stats.playTime, users[i]->stats.sumScores, users[i]->stats.num_games);
        if (!strcmp(getCurrentUser()->creds.name, users[i]->creds.name)){
            menu.uiElements[i] = createLabel((Pos) {200, 100+i*30}, temp, FONTNORMALSCALE*0.75, COLOR_CYAN);
        }
        else{
            menu.uiElements[i] = createLabel((Pos) {200, 100+i*30}, temp, FONTNORMALSCALE*0.75, COLOR_MAGENTA);
        }
    }

    menu.hover_element = -1;
    deactivatePopUp(&menu);
    resetMsgPopUp(&menu);
}

void initscene_leaderboard_menu(){
    // Menu
    menu.enabled = 1;
//    menu.uiElements[4] = createInputField((Pos) {-1, 300}, "", FONTNORMALSCALE, (Scale) {200, 30}, 20);
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->masking = 1;
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->maxLength = PASSWORD_MAXLENGTH;
//    menu.uiElements[5] = createInputField((Pos) {-1, 350}, "Register :", FONTNORMALSCALE,(Scale) {200, 30}, 20);
//    menu.uiElements[6] = createCarousel((Pos) {-1, 400}, "Options :",(char *[]){"Hello", "Test", "Poopak"}, 3, FONTNORMALSCALE);
//    menu.uiElements[7] = createSlider((Pos) {-1, 450}, "Slider :", 50, 0, 100, 5, FONTNORMALSCALE, 50);
//    menu.uiElements[7] = createLabel((Pos) {-1, 460}, "Enter", FONTNORMALSCALE, COLOR_CRIMSON);
    menu.uiElements[7] = createLabel((Pos) {-1, 40}, "Welcome to Roþue!", FONTNORMALSCALE*2, COLOR_CRIMSON);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "leaderboard_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
