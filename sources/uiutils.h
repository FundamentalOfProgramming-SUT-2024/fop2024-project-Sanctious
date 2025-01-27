#ifndef UIUTILS_H
#define UIUTILS_H

#include "renderlibs.h"
#include "config.h"
#include "renderer.h"

extern FT_Library ft;
extern FT_Face face;

typedef enum {
    UI_BUTTON,
    UI_INPUTFIELD,
    UI_LABEL,
    UI_CAROUSEL,
    UI_SLIDER,
    UI_RANKENTY,
    UI_SAVEENTY
} UIElementType;


// Button config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    float fontScale;
    // A -> active, DA -> deactive
    Color Acolor;
    Color DAcolor;

    int isActive;
} ButtonExtra;

// InputField config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    int maxLength;
    char input[MAX_STRING_INPUT];
    float fontScale;

    int boxWidth;
    int defaultBoxWidth;
    int boxHeight;
    int boxOffset;

    // A -> active, DA -> deactive
    Color Acolor;
    Color DAcolor;

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
    int defaultOption;
    char options[MAX_CAROUSEL_OPTIONS][MAX_STR_SIZE];
    float fontScale;

    Color Acolor;
    Color DAcolor;

    int isActive;
} CarouselExtra;

// Slider config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    int minValue;
    int maxValue;
    int curValue;
    int defaultValue;
    int stepValue;
    float fontScale;
    float sliderOffset;

    Color Acolor;
    Color DAcolor;

    int isActive;
} SliderExtra;

// Rank entry config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];

    float fontScale;
    Color color;

    int isActive;
} RankEntryExtra;

// Save entry config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];

    float fontScale;

    Color color;
    int isActive;
} SaveEntryExtra;

// Label config
typedef struct {
    Pos pos;
    char label[MAX_STR_SIZE];
    float fontScale;

    Color color;
} LabelExtra;

typedef struct{
    UIElementType type;
    void* UIExtra; // element specific code

} UIElement;

typedef struct{
    int enabled;
    int hover_element;

    int popUpActive;
    char popUpMsg[MAX_STR_SIZE*4];
    Color popUpColor;

    int num_interactable_elements;
    int num_elements;
    UIElement* uiElements[MAX_MENU_UIELEMENTS];
} Menu;

UIElement* createButton(Pos pos, char* text, float fontScale);
UIElement* createInputField(Pos pos, char* text, float fontScale, Scale boxScale, int boxOffset);
UIElement* createLabel(Pos pos, char* text, float fontScale, Color color);
UIElement* createCarousel(Pos pos, char* text, char** options, int num_options, float fontScale);
UIElement* createSlider(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float fontScale, float sliderOffset);
//UIElement* createRankEntry(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float fontScale, float sliderOffset);
//UIElement* createSaveEntry(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float fontScale, float sliderOffset);

void configureButtonColor(UIElement* button, Color Acolor, Color DAcolor);
void configureInputFieldColor(UIElement* inputField, Color Acolor, Color DAcolor);
void configureCarouselColor(UIElement* carousel, Color Acolor, Color DAcolor);
void configureSliderColor(UIElement* slider, Color Acolor, Color DAcolor);
//void configureRankEntryColor(UIElement* slider, Color Acolor, Color DAcolor);
//void configureSaveEntryColor(UIElement* slider, Color Acolor, Color DAcolor);

char* maskString(char* text, char* output, char mask);
int calculateTextWidth(const char* text, float fontScale);
int calculateTextHeight(const char* text, float fontScale);
void renderMenu(Menu* menu);
void menuBasicHandleKeyboard(Menu* menu, unsigned char key);
void menuBasicHandleSKeyboard(Menu* menu, int key);
void resetMenuFields(Menu* menu);
void activatePopUp(Menu* menu, Color color);
void deactivatePopUp(Menu* menu);
void addMsgToPopUp(Menu* menu, char* string);
void resetMsgPopUp(Menu* menu);

#endif
