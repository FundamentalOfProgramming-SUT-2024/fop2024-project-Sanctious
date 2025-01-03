#ifndef UIUTILS_H
#define UIUTILS_H

#include "config.h"

//widgetmanager->crearte color list of colors
//widgetManager ->getcolor by name
// Rendering & Keyboard handling is not implemented here
typedef struct{
    float r, g, b, a;
} Color;

//create a list of colors const
typedef struct{
    int x, y;
} Pos;

typedef enum {
    UI_BUTTON,
    UI_INPUTFIELD,
    UI_LABEL,
} UIElementType;


typedef struct {
    // Button config
    Pos pos;
    char label[MAX_STR_SIZE];
    int* scale;
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
    int* scale;

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

typedef struct{
    UIElementType type;
    void* UIExtra; // element specific code

} UIElement;

typedef struct{
    int hover_element;

    int num_elements;
    UIElement* uiElements[MAX_UIELEMENTS];
} Menu;

UIElement* createButton();
UIElement* createInputField();
//extern Color* ColorRed;
//extern Color* ColorBlue;
//extern Color* ColorAqua;


#endif
