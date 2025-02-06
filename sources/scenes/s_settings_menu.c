#include "../scene.h"
#include "../renderer.h"
#include "../config.h"
#include "../uiutils.h"
#include "../music.h"

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
            case 2:{
                resetMsgPopUp(&menu);

                CarouselExtra* music = (CarouselExtra *) menu.uiElements[0]->UIExtra;
                CarouselExtra* musicList = (CarouselExtra *) menu.uiElements[1]->UIExtra;
                
                stopMusic();

                switch(musicList->curOption){
                    case 0:
                        playMusic("musics/starfighter.mp3");
                        break;
                    case 1:
                        playMusic("musics/steinsgate.mp3");
                        break;
                }


                if (music->curOption == 1){
                    stopMusic();
                }

                addMsgToPopUp(&menu, "Changed music successfully!");
                activatePopUp(&menu, COLOR_LIME_GREEN);
                break;
            }
            // Back
            case 3:
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
    resetMenuFields(&menu);
}

void initscene_settings_menu(){
    // Menu
    menu.enabled = 1;
    menu.num_elements = 5;
    menu.num_interactable_elements = 4;

    menu.uiElements[0] = createCarousel((Pos) {RWINDOW_WIDTH/2-140, 150}, "Music :",(char *[]){"On", "Off"}, 2, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[0], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[1] = createCarousel((Pos) {RWINDOW_WIDTH/2-140, 220}, "Music list :",(char *[]){"Starfighter", "Steins:Gate"}, 2, FONTNORMALSCALE);
    configureCarouselColor(menu.uiElements[1], COLOR_GRAY, COLOR_CYAN);

    menu.uiElements[2] = createButton((Pos) {-1, 290}, "Apply", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[2], COLOR_GRAY, COLOR_CYAN);
    
    menu.uiElements[3] = createButton((Pos) {-1, 360}, "Back", FONTNORMALSCALE);
    configureButtonColor(menu.uiElements[3], COLOR_GRAY, COLOR_RUBY);
    
    menu.uiElements[4] = createLabel((Pos) {-1, 70}, "Settings", FONTNORMALSCALE*1.5, COLOR_EMERALD);
//    menu.uiElements[4] = createInputField((Pos) {-1, 300}, "", FONTNORMALSCALE, (Scale) {300, 30}, 20);
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->masking = 1;
//    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->maxLength = PASSWORD_MAXLENGTH;
//    menu.uiElements[5] = createInputField((Pos) {-1, 350}, "Register :", FONTNORMALSCALE,(Scale) {300, 30}, 20);
//    menu.uiElements[6] = createCarousel((Pos) {-1, 400}, "Options :",(char *[]){"Hello", "Test", "Poopak"}, 3, FONTNORMALSCALE);
//    menu.uiElements[7] = createSlider((Pos) {-1, 450}, "Slider :", 50, 0, 100, 5, FONTNORMALSCALE, 50);
//    menu.uiElements[7] = createLabel((Pos) {-1, 460}, "Enter", FONTNORMALSCALE, COLOR_CRIMSON);

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "settings_menu");

    scene->onEnter = onEnter;
    scene->onExit = onExit;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
