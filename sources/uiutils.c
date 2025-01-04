#include "uiutils.h"
#include "stdlib.h"
#include <strings.h>
#include "renderer.h"

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

char* maskString(char* text, char* output, char mask){
    for (int i = 0; i < strlen(text); i++) {
        output[i] = mask;
    }
    output[strlen(text)] = '\0';
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
    inpConfig->masking = 0;
    inpConfig->isActive = 0;
    inpConfig->maxLength = 20;

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

// Carousel creation
UIElement* createCarousel(Pos pos, char* text, char** options, int num_options, float scale){
    UIElement* carousel = (UIElement *) malloc(1 * sizeof(UIElement));
    carousel->type = UI_CAROUSEL;
    CarouselExtra* carouselConfig = (CarouselExtra *) malloc(1 * sizeof(CarouselExtra));
    carouselConfig->scale = scale;

    strcpy(carouselConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, scale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, scale))/2; // Text is rendering upwards

    carouselConfig->pos = (Pos) {x, y};
    carouselConfig->Acolor = (Color) {0.5, 0.5, 0.5, 1};
    carouselConfig->DAcolor = (Color) {0, 0.5, 0.5, 1};

    carouselConfig->curOption = 0;
    carouselConfig->num_options = num_options;
    for (int i = 0; i < num_options; i++){
        strcpy(carouselConfig->options[i], options[i]);
    }

    carousel->UIExtra = (void *) carouselConfig;

    return carousel;
}

// Slider creation
UIElement* createSlider(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float scale, float sliderOffset){
    UIElement* slider = (UIElement *) malloc(1 * sizeof(UIElement));
    slider->type = UI_SLIDER;
    SliderExtra* sliderConfig = (SliderExtra *) malloc(1 * sizeof(SliderExtra));
    sliderConfig->scale = scale;

    strcpy(sliderConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, scale)-sliderOffset)/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, scale))/2; // Text is rendering upwards

    sliderConfig->pos = (Pos) {x, y};
    sliderConfig->sliderOffset = sliderOffset;
    sliderConfig->curValue = curValue;
    sliderConfig->minValue = minValue;
    sliderConfig->maxValue = maxValue;
    sliderConfig->stepValue = stepValue;
    sliderConfig->Acolor = (Color) {0.5, 0.5, 0.5, 1};
    sliderConfig->DAcolor = (Color) {0, 0.5, 0.5, 1};

    slider->UIExtra = (void *) sliderConfig;

    return slider;
}

void renderMenu(Menu* menu) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // Draw a smooth 2D box

    for (int i = 0; i < menu->num_elements; i++){
        // Button
        if(menu->uiElements[i]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Input field
        else if(menu->uiElements[i]->type == UI_INPUTFIELD){
            InputFieldExtra* extra = (InputFieldExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
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

            char output[MAX_STR_SIZE];
            if (extra->masking){
                maskString(extra->input, output, '*');
                renderString(_x, extra->pos.y, output, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
            }
            else{
                renderString(_x, extra->pos.y, extra->input, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
            }
        }
        // Carousel
        else if(menu->uiElements[i]->type == UI_CAROUSEL){
            CarouselExtra* extra = (CarouselExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
            renderString(extra->pos.x+120, extra->pos.y, extra->options[extra->curOption], extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Slider
        else if(menu->uiElements[i]->type == UI_SLIDER){
            SliderExtra* extra = (SliderExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->Acolor.r, extra->Acolor.g, extra->Acolor.b, extra->Acolor.a);
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);

            char str[MAX_STR_SIZE];
            sprintf(str, "%d", extra->curValue);
            renderString(extra->pos.x+calculateTextWidth(extra->label, extra->scale)+extra->sliderOffset, extra->pos.y, str, extra->scale, extra->DAcolor.r, extra->DAcolor.g, extra->DAcolor.b, extra->DAcolor.a);
        }
        // Label
        else if(menu->uiElements[i]->type == UI_LABEL){
            LabelExtra* extra = (LabelExtra *) menu->uiElements[i]->UIExtra;
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->scale, extra->color.r, extra->color.g, extra->color.b, extra->color.a);
        }
    }
}
