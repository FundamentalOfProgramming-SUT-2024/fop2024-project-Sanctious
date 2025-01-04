#ifndef UIUTILS_H
#define UIUTILS_H

#include "renderlibs.h"
#include "config.h"

extern FT_Library ft;
extern FT_Face face;

typedef struct{
    float r, g, b, a;
} Color;

typedef struct{
    float x, y;
} Pos;

typedef struct{
    float w, h;
} Scale;

typedef enum {
    UI_BUTTON,
    UI_INPUTFIELD,
    UI_LABEL,
    UI_CAROUSEL,
    UI_SLIDER,
} UIElementType;


// Button config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    float scale;
    // A -> active, DA -> deactive
    Color Acolor;
    Color Abgcolor;
    Color DAcolor;
    Color DAbgcolor;

    int isActive;
} ButtonExtra;

// InputField config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    int maxLength;
    char input[MAX_STR_SIZE];
    float scale;

    int boxWidth;
    int boxHeight;
    int boxOffset;

    // A -> active, DA -> deactive
    Color Acolor;
    Color Abgcolor;
    Color DAcolor;
    Color DAbgcolor;

    int masking;
//    char maskChar;
    int isActive;
} InputFieldExtra;

// Carousel config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    int num_options;
    int curOption;
    char* options[MAX_STR_SIZE];
    float scale;

    Color color;

    int isActive;
} CarouselExtra;

// Slider config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    int minValue;
    int maxValue;
    int curValue;
    float scale;

    Color color;

    int isActive;
} SliderExtra;

// Label config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    float scale;

    Color color;
} LabelExtra;

typedef struct{
    UIElementType type;
    void* UIExtra; // element specific code

} UIElement;

typedef struct{
    int hover_element;

    int num_interactable_elements;
    int num_elements;
    UIElement* uiElements[MAX_UIELEMENTS];
} Menu;

UIElement* createButton(Pos pos, char* text, float scale);
UIElement* createInputField(Pos pos, char* text, float scale, Scale boxScale, int boxOffset);
UIElement* createLabel(Pos pos, char* text, float scale, Color color);
UIElement* createCarousel(Pos pos, char* text, char* options, int num_options, float scale, Color color);
UIElement* createSlider(Pos pos, char* text, float scale, Color color);
char* maskString(char* text, char* output, char mask);
int calculateTextWidth(const char* text, float scale);
int calculateTextHeight(const char* text, float scale);


#endif
