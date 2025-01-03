#include "uiutils.h"
#include "stdlib.h"
#include <strings.h>

extern FT_Library ft;
extern FT_Face face;
//Color* ColorRed = NULL;
//
//void initializeColors(){
//    *ColorRed = (Color) {0.5, 0.5, 0.5, 1};
//
//}


// Render width
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

// Render height
int calculateTextHeight(FT_Face face, const char* text, float scale) {
    int maxTop = 0;
    int minBottom = 0;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            continue;
        }
        FT_GlyphSlot glyph = face->glyph;

        int top = glyph->bitmap_top * scale;
        int bottom = top - (glyph->bitmap.rows * scale);

        if (top > maxTop) maxTop = top;
        if (bottom < minBottom) minBottom = bottom;
    }
    return maxTop - minBottom;
}

UIElement* createButton(Pos pos){
    UIElement* btn = (UIElement *) malloc(1 * sizeof(UIElement));
    btn->type = UI_BUTTON;
    ButtonExtra* btnConfig = (ButtonExtra *) malloc(1 * sizeof(ButtonExtra));

    btnConfig->pos = pos;

    strcpy(btnConfig->label, "AMOGUGGSuDAUSD");

//    Color* red = (Color *) malloc(1 * sizeof(Color));
//    *red = (Color) {0.5, 0.5, 0.5, 1};
//    Color* blue = (Color *) malloc(1 * sizeof(Color));
//    *blue = (Color) {0, 0.5, 0.5, 1};

    btnConfig->Acolor = (Color) {0.5, 0.5, 0.5, 1};
    btnConfig->DAcolor = (Color) {0, 0.5, 0.5, 1};
    btnConfig->isActive = 0;

    btn->UIExtra = (void *) btnConfig;

    return btn;
}

UIElement* createInputField(Pos pos){
    UIElement* inp = (UIElement *) malloc(1 * sizeof(UIElement));
    inp->type = UI_INPUTFIELD;
    InputFieldExtra* inpConfig = (InputFieldExtra *) malloc(1 * sizeof(InputFieldExtra));

    inpConfig->pos = pos;
    inpConfig->boxOffset = 20;
    inpConfig->boxWidth = 200;
    inpConfig->boxHeight = 20;

    strcpy(inpConfig->label, "AMOGUGGSuDAUSD");
    strcpy(inpConfig->input, "");

//    Color* red = (Color *) malloc(1 * sizeof(Color));
//    *red = (Color) {0.5, 0.5, 0.5, 1};
//    Color* blue = (Color *) malloc(1 * sizeof(Color));
//    *blue = (Color) {0, 0.5, 0.5, 1};

    inpConfig->Acolor = (Color) {0.5, 0.5, 0.5, 1};
    inpConfig->DAcolor = (Color) {0, 0.5, 0.5, 1};
    inpConfig->isActive = 0;

    inp->UIExtra = (void *) inpConfig;

    return inp;
}
