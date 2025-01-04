#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"
#include "uiutils.h"
#include <ctype.h>

static Menu menu;

static void render() {

    glClear(GL_COLOR_BUFFER_BIT);
    renderMenu(&menu);
    glFlush();
}

static void processSKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_F2){
//        void * temp = (menu.uiElements[0]->UIExtra);
//        ((ButtonExtra *) temp)->isActive = !((ButtonExtra *) temp)->isActive;
        changeScene(getSceneByID("game"));
	}
	else if (key == GLUT_KEY_DOWN){
        if (menu.hover_element < menu.num_interactable_elements-1) menu.hover_element += 1;
        else menu.hover_element = 0;
	}
	else if (key == GLUT_KEY_UP){
       if (menu.hover_element > 0) menu.hover_element -= 1;
       else menu.hover_element = menu.num_interactable_elements-1;
	}
	else if (key == GLUT_KEY_RIGHT){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1){
            if (menu.uiElements[menu.hover_element]->type == UI_CAROUSEL){
                CarouselExtra* extra = (CarouselExtra *) menu.uiElements[menu.hover_element]->UIExtra;
                if (extra->curOption < extra->num_options-1) extra->curOption += 1;
                else extra->curOption = 0;
            }
            else if (menu.uiElements[menu.hover_element]->type == UI_SLIDER){
                SliderExtra* extra = (SliderExtra *) menu.uiElements[menu.hover_element]->UIExtra;
                if (extra->curValue < extra->maxValue) extra->curValue += extra->stepValue;
            }
        }
	}

	else if (key == GLUT_KEY_LEFT){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1){
            if (menu.uiElements[menu.hover_element]->type == UI_CAROUSEL){
                CarouselExtra* extra = (CarouselExtra *) menu.uiElements[menu.hover_element]->UIExtra;
                if (extra->curOption > 0) extra->curOption -= 1;
                else extra->curOption = extra->num_options-1;
            }
            else if (menu.uiElements[menu.hover_element]->type == UI_SLIDER){
                SliderExtra* extra = (SliderExtra *) menu.uiElements[menu.hover_element]->UIExtra;
                if (extra->curValue > extra->minValue) extra->curValue -= extra->stepValue;
            }
        }
	}

}

static void processKeyboard(unsigned char key, int x, int y) {

    // key == 8 -> backspace
	if (isprint(key) || key == 8){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1)
        if (menu.uiElements[menu.hover_element]->type == UI_INPUTFIELD){
            InputFieldExtra* extra = (InputFieldExtra *) menu.uiElements[menu.hover_element]->UIExtra;
            int len = strlen(extra->input);
            // Backspace
            if (key == 8){
                extra->input[len-1] = '\0';
            }
            // Other characters
            else{
                if (len < extra->maxLength){
                    extra->input[len] = key;
                    extra->input[len+1] = '\0';
                }
            }
            // Automatic box resizing
            extra->boxWidth = 5+calculateTextWidth(extra->input, extra->scale);
            if (extra->masking){
                char output[MAX_STR_SIZE];
                maskString(extra->input, output, '*');
                extra->boxWidth = 5+calculateTextWidth(output, extra->scale);
            }
        }
	}

	// Manually handle buttons
	// key == 13 -> Enter key
    else if (key == 13){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1)
        if (menu.uiElements[menu.hover_element]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu.uiElements[menu.hover_element]->UIExtra;
            extra->Acolor = (Color ) {0.1f, 0.1f, 0.1f, 1};

        }
    }

}

void initscene_main_menu(){
    // Menu
    menu.num_elements = 10;
    menu.num_interactable_elements = 8;
    menu.hover_element = -1;
    menu.uiElements[0] = createButton((Pos) {-1, 100}, "Hello!", FONTNORMALSCALE);
    menu.uiElements[1] = createButton((Pos) {-1, 150}, "test!", FONTNORMALSCALE);
    menu.uiElements[2] = createButton((Pos) {-1, 200}, "AMOGUASUDAUSD", FONTNORMALSCALE*2);
    menu.uiElements[3] = createButton((Pos) {-1, 250}, "SUSSY baka!!", FONTNORMALSCALE);
    menu.uiElements[4] = createInputField((Pos) {-1, 300}, "Login :", FONTNORMALSCALE, (Scale) {200, 30}, 20);
    ((InputFieldExtra *) menu.uiElements[4]->UIExtra)->masking = 1;
    menu.uiElements[5] = createInputField((Pos) {-1, 350}, "Register :", FONTNORMALSCALE,(Scale) {200, 30}, 20);
    menu.uiElements[6] = createCarousel((Pos) {-1, 400}, "Options :",(char *[]){"Hello", "Test", "Poopak"}, 3, FONTNORMALSCALE);
    menu.uiElements[7] = createSlider((Pos) {-1, 450}, "Slider :", 50, 0, 100, 5, FONTNORMALSCALE, 50);
    menu.uiElements[8] = createLabel((Pos) {-1, 500}, "Enter", FONTNORMALSCALE, (Color) {0.7, 0, 0, 1});
    menu.uiElements[9] = createLabel((Pos) {-1, 40}, "Welcome to Roþue!", FONTNORMALSCALE*2, (Color) {0.7, 0, 0, 1});

    // Scene
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "main_menu");

    scene->onEnter = NULL;
    scene->onExit = NULL;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
