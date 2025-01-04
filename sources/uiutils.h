#ifndef UIUTILS_H
#define UIUTILS_H

#include "renderlibs.h"
#include "config.h"

extern FT_Library ft;
extern FT_Face face;
//widgetmanager->crearte color list of colors
//widgetManager ->getcolor by name
// Rendering & Keyboard handling is not implemented here
typedef struct{
    float r, g, b, a;
} Color;

//create a list of colors const
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
} UIElementType;


typedef struct {
    // Button config
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

typedef struct {
    // Button config
    Pos pos;
    char label[MAX_STR_SIZE];
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

    int isActive;
} InputFieldExtra;

typedef struct {
    // Button config
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
int calculateTextWidth(const char* text, float scale);
int calculateTextHeight(const char* text, float scale);
//extern Color* ColorRed;
//extern Color* ColorBlue;
//extern Color* ColorAqua;


#endif
