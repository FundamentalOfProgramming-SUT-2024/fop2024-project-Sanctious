#include "uiutils.h"
#include "stdlib.h"
#include <strings.h>
#include "renderer.h"

extern FT_Library ft;
extern FT_Face face;

// Render width
int calculateTextWidth(const char* text, float fontScale) {
    int width = 0;
    for (const char* p = text; *p; p++) {
         if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            continue;
        }
        width += (face->glyph->advance.x >> 6) * fontScale;
    }
    return width;
}

// Render height
int calculateTextHeight(const char* text, float fontScale) {
    int maxTop = 0;
    int minBottom = 0;
    for (const char* p = text; *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            continue;
        }
        FT_GlyphSlot glyph = face->glyph;

        int top = glyph->bitmap_top * fontScale;
        int bottom = top - (glyph->bitmap.rows * fontScale);

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
UIElement* createButton(Pos pos, char* text, float fontScale){
    UIElement* btn = (UIElement *) malloc(1 * sizeof(UIElement));
    btn->type = UI_BUTTON;
    ButtonExtra* btnConfig = (ButtonExtra *) malloc(1 * sizeof(ButtonExtra));
    btnConfig->fontScale = fontScale;


    strcpy(btnConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, fontScale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, fontScale))/2; // Text is rendering upwards

    btnConfig->pos = (Pos) {x, y};
    btnConfig->Acolor = COLOR_GRAY;
    btnConfig->DAcolor = COLOR_CYAN;
    btnConfig->isActive = 0;

    btn->UIExtra = (void *) btnConfig;

    return btn;
}

// Input field creation
UIElement* createInputField(Pos pos, char* text, float fontScale, Scale boxScale, int boxOffset){
    UIElement* inp = (UIElement *) malloc(1 * sizeof(UIElement));
    inp->type = UI_INPUTFIELD;
    InputFieldExtra* inpConfig = (InputFieldExtra *) malloc(1 * sizeof(InputFieldExtra));
    inpConfig->fontScale = fontScale;

    inpConfig->boxOffset = boxOffset;
    inpConfig->boxWidth = boxScale.w;
    inpConfig->boxHeight = boxScale.h;

    strcpy(inpConfig->label, text);
    strcpy(inpConfig->input, "");

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, fontScale)-inpConfig->boxWidth-inpConfig->boxOffset)/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, fontScale))/2; // Text is rendering upwards

    inpConfig->pos = (Pos) {x, y};
    inpConfig->Acolor = COLOR_GRAY;
    inpConfig->DAcolor = COLOR_CYAN;
    inpConfig->masking = 0;
    inpConfig->isActive = 0;
    inpConfig->maxLength = 20;

    inp->UIExtra = (void *) inpConfig;

    return inp;
}

// Label creation
UIElement* createLabel(Pos pos, char* text, float fontScale, Color color){
    UIElement* label = (UIElement *) malloc(1 * sizeof(UIElement));
    label->type = UI_LABEL;
    LabelExtra* labelConfig = (LabelExtra *) malloc(1 * sizeof(LabelExtra));
    labelConfig->fontScale = fontScale;

    strcpy(labelConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, fontScale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, fontScale))/2; // Text is rendering upwards

    labelConfig->pos = (Pos) {x, y};
    labelConfig->color = color;

    label->UIExtra = (void *) labelConfig;

    return label;
}

// Carousel creation
UIElement* createCarousel(Pos pos, char* text, char** options, int num_options, float fontScale){
    UIElement* carousel = (UIElement *) malloc(1 * sizeof(UIElement));
    carousel->type = UI_CAROUSEL;
    CarouselExtra* carouselConfig = (CarouselExtra *) malloc(1 * sizeof(CarouselExtra));
    carouselConfig->fontScale = fontScale;

    strcpy(carouselConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, fontScale))/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, fontScale))/2; // Text is rendering upwards

    carouselConfig->pos = (Pos) {x, y};
    carouselConfig->Acolor = COLOR_GRAY;
    carouselConfig->DAcolor = COLOR_CYAN;

    carouselConfig->curOption = 0;
    carouselConfig->num_options = num_options;
    for (int i = 0; i < num_options; i++){
        strcpy(carouselConfig->options[i], options[i]);
    }

    carousel->UIExtra = (void *) carouselConfig;

    return carousel;
}

// Slider creation
UIElement* createSlider(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float fontScale, float sliderOffset){
    UIElement* slider = (UIElement *) malloc(1 * sizeof(UIElement));
    slider->type = UI_SLIDER;
    SliderExtra* sliderConfig = (SliderExtra *) malloc(1 * sizeof(SliderExtra));
    sliderConfig->fontScale = fontScale;

    strcpy(sliderConfig->label, text);

    float x = pos.x;
    float y = pos.y;
    // Center
    if (x == -1) x = (RWINDOW_WIDTH-calculateTextWidth(text, fontScale)-sliderOffset)/2;
    if (y == -1) y = (RWINDOW_HEIGHT+calculateTextHeight(text, fontScale))/2; // Text is rendering upwards

    sliderConfig->pos = (Pos) {x, y};
    sliderConfig->sliderOffset = sliderOffset;
    sliderConfig->curValue = curValue;
    sliderConfig->minValue = minValue;
    sliderConfig->maxValue = maxValue;
    sliderConfig->stepValue = stepValue;
    sliderConfig->Acolor = COLOR_GRAY;
    sliderConfig->DAcolor = COLOR_CYAN;

    slider->UIExtra = (void *) sliderConfig;

    return slider;
}

void renderMenu(Menu* menu) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    for (int i = 0; i < menu->num_elements; i++){
        // Button
        if(menu->uiElements[i]->type == UI_BUTTON){
            ButtonExtra* extra = (ButtonExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i){
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);
        }
        // Input field
        else if(menu->uiElements[i]->type == UI_INPUTFIELD){
            InputFieldExtra* extra = (InputFieldExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i){
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);
            glLineWidth(1.5f);
            glColor3f(0.5f, 0.5f, 0.5f);
            int _x = extra->pos.x+calculateTextWidth(extra->label, extra->fontScale)+extra->boxOffset;
            glBegin(GL_LINE_LOOP);
                glVertex2f(_x-INPUTBOX_LEFTMARGIN, extra->pos.y+INPUTBOX_BOTTOMMARGIN);
                glVertex2f(_x+extra->boxWidth+INPUTBOX_RIGHTMARGIN, extra->pos.y+INPUTBOX_BOTTOMMARGIN);
                glVertex2f(_x+extra->boxWidth+INPUTBOX_RIGHTMARGIN, extra->pos.y-extra->boxHeight-INPUTBOX_TOPMARGIN);
                glVertex2f(_x-INPUTBOX_LEFTMARGIN, extra->pos.y-extra->boxHeight-INPUTBOX_TOPMARGIN);
            glEnd();

            char output[MAX_STR_SIZE];
            if (extra->masking){
                maskString(extra->input, output, PASSWORDMASK_CHAR);
                renderString(_x, extra->pos.y, output, extra->fontScale, extra->DAcolor);
            }
            else{
                renderString(_x, extra->pos.y, extra->input, extra->fontScale, extra->DAcolor);
            }

        }
        // Carousel
        else if(menu->uiElements[i]->type == UI_CAROUSEL){
            CarouselExtra* extra = (CarouselExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i){
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);
            renderString(extra->pos.x+120, extra->pos.y, "<", extra->fontScale, extra->Acolor);
            renderString(extra->pos.x+140, extra->pos.y, extra->options[extra->curOption], extra->fontScale, extra->DAcolor);
            renderString(extra->pos.x+240, extra->pos.y, ">", extra->fontScale, extra->Acolor);


        }
        // Slider
        else if(menu->uiElements[i]->type == UI_SLIDER){
            SliderExtra* extra = (SliderExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i){
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);

            char str[MAX_STR_SIZE];
            sprintf(str, "%d", extra->curValue);
            renderString(extra->pos.x+100, extra->pos.y, "<", extra->fontScale, extra->Acolor);
            renderString(extra->pos.x+calculateTextWidth(extra->label, extra->fontScale)+extra->sliderOffset, extra->pos.y, str, extra->fontScale, extra->DAcolor);
            renderString(extra->pos.x+200, extra->pos.y, ">", extra->fontScale, extra->Acolor);
        }
        // Label
        else if(menu->uiElements[i]->type == UI_LABEL){
            LabelExtra* extra = (LabelExtra *) menu->uiElements[i]->UIExtra;
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->color);
        }
    }
}
