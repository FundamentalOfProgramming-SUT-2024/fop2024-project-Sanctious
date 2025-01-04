#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"
#include "uiutils.h"
#include <ctype.h>

static Menu menu;

static void render() {

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // Draw a smooth 2D box


    for (int i = 0; i < menu.num_elements; i++){
        // Button
        if(menu.uiElements[i]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu.uiElements[i]->UIExtra;
            if (menu.hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Input field
        if(menu.uiElements[i]->type == UI_INPUTFIELD){
            InputFieldExtra* extra = (InputFieldExtra *) menu.uiElements[i]->UIExtra;
            if (menu.hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
            glLineWidth(1.5f);
            glColor3f(0.5f, 0.5f, 0.5f);
            int _x = extra->pos.x+calculateTextWidth(extra->label, extra->scale)+extra->boxOffset;
            glBegin(GL_LINE_LOOP);
                glVertex2f(_x-5, extra->pos.y+7);
                glVertex2f(_x+extra->boxWidth+5, extra->pos.y+7);
                glVertex2f(_x+extra->boxWidth+5, extra->pos.y-extra->boxHeight-5);
                glVertex2f(_x-5, extra->pos.y-extra->boxHeight-5);
            glEnd();
            renderString(_x, extra->pos.y, extra->input, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Label
        if(menu.uiElements[i]->type == UI_LABEL){
            LabelExtra* extra = (LabelExtra *) menu.uiElements[i]->UIExtra;
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->color.r, extra->color.g, extra->color.b, extra->color.a);
        }
    }


    renderString(0, 20, "Welcome to rogue!", FONTNORMALSCALE, 0.5f, 0.1f, 0.9f, 1.0f);

    glFlush();
}

static void processSKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_F3){
        menu.num_elements = 7;
        menu.hover_element = -1;
        menu.uiElements[0] = createButton((Pos) {-1, 50}, "Hello!", FONTNORMALSCALE);
    //    ((ButtonExtra *) menu.uiElements[0]->UIExtra)->isActive = 1;
        menu.uiElements[1] = createButton((Pos) {-1, 100}, "test!", FONTNORMALSCALE);
        menu.uiElements[2] = createButton((Pos) {-1, 150}, "AMOGUASUDAUSD", FONTNORMALSCALE);
        menu.uiElements[3] = createButton((Pos) {-1, 200}, "SUSSY baka!!", FONTNORMALSCALE);
        menu.uiElements[4] = createInputField((Pos) {-1, 250}, "Login:", FONTNORMALSCALE, (Scale) {100, 30}, 20);
        menu.uiElements[5] = createInputField((Pos) {-1, 300}, "Register:", FONTNORMALSCALE,(Scale) {100, 30}, 20);
        menu.uiElements[6] = createLabel((Pos) {-1, 350}, "Enter", FONTNORMALSCALE, (Color) {0.7, 0, 0, 1});
    }
	if (key == GLUT_KEY_F2){
//        void * temp = (menu.uiElements[0]->UIExtra);
//        ((ButtonExtra *) temp)->isActive = !((ButtonExtra *) temp)->isActive;
        changeScene(getSceneByID("game"));
	}
	if (key == GLUT_KEY_DOWN){
        if (menu.hover_element < menu.num_elements-1) menu.hover_element += 1;
        else menu.hover_element = 0;
	}
	if (key == GLUT_KEY_UP){
       if (menu.hover_element > 0) menu.hover_element -= 1;
       else menu.hover_element = menu.num_elements-1;
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
                extra->input[len] = key;
                extra->input[len+1] = '\0';
            }
            extra->boxWidth = 5+calculateTextWidth(extra->input, extra->scale);
        }
	}

	// Manually handle buttons
	// key == 13 -> Enter key
    if (key == 13){
        if (menu.hover_element >= 0 && menu.hover_element <= menu.num_elements-1)
        if (menu.uiElements[menu.hover_element]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu.uiElements[menu.hover_element]->UIExtra;
            extra->Acolor = (Color ) {0.1f, 0.1f, 0.1f, 1};

        }
    }

}
void initscene_leaderboard_menu(){
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "leaderboard_menu");

    scene->onEnter = NULL;
    scene->onExit = NULL;
    scene->onKeypress = NULL;
    scene->onSpecialKeypress = processSKeyboard;
    scene->onKeypress = processKeyboard;


    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
