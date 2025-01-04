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
int calculateTextWidth(const char* text, float scale) {
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
int calculateTextHeight(const char* text, float scale) {
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

// Button creation
UIElement* createButton(Pos pos, char* text, float scale){
    UIElement* btn = (UIElement *) malloc(1 * sizeof(UIElement));
    btn->type = UI_BUTTON;
    ButtonExtra* btnConfig = (ButtonExtra *) malloc(1 * sizeof(ButtonExtra));
    btnConfig->scale = scale;


    strcpy(btnConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, scale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, scale))/2; // Text is rendering upwards

    btnConfig->pos = (Pos) {x, y};
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

// Input field creation
UIElement* createInputField(Pos pos, char* text, float scale, Scale boxScale, int boxOffset){
    UIElement* inp = (UIElement *) malloc(1 * sizeof(UIElement));
    inp->type = UI_INPUTFIELD;
    InputFieldExtra* inpConfig = (InputFieldExtra *) malloc(1 * sizeof(InputFieldExtra));
    inpConfig->scale = scale;

    inpConfig->boxOffset = boxOffset;
    inpConfig->boxWidth = boxScale.w;
    inpConfig->boxHeight = boxScale.h;

    strcpy(inpConfig->label, text);
    strcpy(inpConfig->input, "");

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, scale)-inpConfig->boxWidth-inpConfig->boxOffset)/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, scale))/2; // Text is rendering upwards

    inpConfig->pos = (Pos) {x, y};
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

// Label creation
UIElement* createLabel(Pos pos, char* text, float scale, Color color){
    UIElement* label = (UIElement *) malloc(1 * sizeof(UIElement));
    label->type = UI_LABEL;
    LabelExtra* labelConfig = (LabelExtra *) malloc(1 * sizeof(LabelExtra));
    labelConfig->scale = scale;

    strcpy(labelConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, scale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, scale))/2; // Text is rendering upwards

    labelConfig->pos = (Pos) {x, y};
    labelConfig->color = color;

    label->UIExtra = (void *) labelConfig;

    return label;
}
