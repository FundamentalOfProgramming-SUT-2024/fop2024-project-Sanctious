#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"
#include "uiutils.h"
#include <ctype.h>

static Menu menu;

extern FT_Library ft;
extern FT_Face face;
//
//menu->buttons
//menu->num buttons

//label

// temp fix
int calculateTextWidth(FT_Face face, const char* text, float scale);

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
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Input field
        if(menu.uiElements[i]->type == UI_INPUTFIELD){
            InputFieldExtra* extra = (InputFieldExtra *) menu.uiElements[i]->UIExtra;
            if (menu.hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
            glLineWidth(1.5f);
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINE_LOOP);
                glVertex2f(extra->pos.x+calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f)+extra->boxOffset, extra->pos.y);
                glVertex2f(extra->pos.x+extra->boxWidth+calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f)+extra->boxOffset, extra->pos.y);
                glVertex2f(extra->pos.x+extra->boxWidth+calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f)+extra->boxOffset, extra->pos.y-extra->boxHeight);
                glVertex2f(extra->pos.x+calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f)+extra->boxOffset, extra->pos.y-extra->boxHeight);
            glEnd();
            renderString(extra->pos.x+calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f)+extra->boxOffset, extra->pos.y, extra->input, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
    }


    renderString(0, 20, "Welcome to rogue!", 0.5f, 0.1f, 0.9f, 1.0f);

    glFlush();
}


// Function to calculate the width of a string
int calculateTextWidth(FT_Face face, const char* text, float scale) {
    int width = 0;
    for (const char* p = text; *p; p++) {
         if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            continue;
        }

        width += (face->glyph->advance.x >> 6) * scale;

    }

    return width;
}

// Function to calculate the height of a string
int calculateTextHeight(FT_Face face, const char* text, float scale) {
    int maxTop = 0;
    int minBottom = 0;

    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            continue; // Skip characters that fail to load
        }

        FT_GlyphSlot glyph = face->glyph;

        // Calculate the top and bottom positions of the glyph
        int top = glyph->bitmap_top * scale;
        int bottom = top - (glyph->bitmap.rows * scale);

        // Update the overall maxTop and minBottom
        if (top > maxTop) maxTop = top;
        if (bottom < minBottom) minBottom = bottom;
    }

    return maxTop - minBottom; // Total height
}


static void processSKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_F3){
        menu.num_elements = 5;
        menu.hover_element = -1;
        int x = WINDOW_WIDTH+XBUFFER_ZONE+EXTRA_BUFFER-calculateTextWidth(face, "AMOGUGGSuDAUSD", 0.5f);
        menu.uiElements[0] = createButton((Pos) {x/2, 60});
    //    ((ButtonExtra *) menu.uiElements[0]->UIExtra)->isActive = 1;
        menu.uiElements[1] = createButton((Pos) {x/2, 100});
        menu.uiElements[2] = createButton((Pos) {x/2, 140});
        menu.uiElements[3] = createButton((Pos) {x/2, 180});
        menu.uiElements[4] = createInputField((Pos) {x/2-100, 220});
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
        }
	}
}

void initscene_main_menu(){
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
