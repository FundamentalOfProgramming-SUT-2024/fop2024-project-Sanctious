#include "uiutils.h"
#include "stdlib.h"
#include <strings.h>
#include "renderer.h"
#include <ctype.h>

extern FT_Library ft;
extern FT_Face face;

// Render width
int calculateTextWidth(const char* text, float fontScale)
{
    int width = 0;
    for (const char* p = text; *p; p++)
    {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL))
        {
            continue;
        }
        width += (face->glyph->advance.x >> 6) * fontScale;
    }
    return width;
}

// Render height
int calculateTextHeight(const char* text, float fontScale)
{
    int maxTop = 0;
    int minBottom = 0;
    for (const char* p = text; *p; p++)
    {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL))
        {
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

char* maskString(char* text, char* output, char mask)
{
    for (int i = 0; i < strlen(text); i++)
    {
        output[i] = mask;
    }
    output[strlen(text)] = '\0';
}

// Button creation
UIElement* createButton(Pos pos, char* text, float fontScale)
{
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

    btnConfig->pos = (Pos)
    {
        x, y
    };
    btnConfig->Acolor = COLOR_GRAY;
    btnConfig->DAcolor = COLOR_CYAN;
    btnConfig->isActive = 0;

    btn->UIExtra = (void *) btnConfig;

    return btn;
}

void configureButtonColor(UIElement* button, Color Acolor, Color DAcolor)
{
    ButtonExtra* extra = (ButtonExtra *) button->UIExtra;
    extra->Acolor = Acolor;
    extra->DAcolor = DAcolor;
}

// Input field creation
UIElement* createInputField(Pos pos, char* text, float fontScale, Scale boxScale, int boxOffset)
{
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

    inpConfig->pos = (Pos)
    {
        x, y
    };

    inpConfig->defaultBoxWidth = boxScale.w;
    inpConfig->Acolor = COLOR_GRAY;
    inpConfig->DAcolor = COLOR_CYAN;
    inpConfig->masking = 0;
    inpConfig->isActive = 0;
    inpConfig->maxLength = 20;

    inp->UIExtra = (void *) inpConfig;

    return inp;
}

void configureInputFieldColor(UIElement* inputField, Color Acolor, Color DAcolor)
{
    InputFieldExtra* extra = (InputFieldExtra *) inputField->UIExtra;
    extra->Acolor = Acolor;
    extra->DAcolor = DAcolor;
}

// Label creation
UIElement* createLabel(Pos pos, char* text, float fontScale, Color color)
{
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

    labelConfig->pos = (Pos)
    {
        x, y
    };
    labelConfig->color = color;

    label->UIExtra = (void *) labelConfig;

    return label;
}

// Carousel creation
UIElement* createCarousel(Pos pos, char* text, char** options, int num_options, float fontScale)
{
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

    carouselConfig->pos = (Pos)
    {
        x, y
    };
    carouselConfig->Acolor = COLOR_GRAY;
    carouselConfig->DAcolor = COLOR_CYAN;
    carouselConfig->defaultOption = 0;
    carouselConfig->curOption = 0;
    carouselConfig->num_options = num_options;
    for (int i = 0; i < num_options; i++)
    {
        strcpy(carouselConfig->options[i], options[i]);
    }

    carousel->UIExtra = (void *) carouselConfig;

    return carousel;
}

void configureCarouselColor(UIElement* carousel, Color Acolor, Color DAcolor)
{
    CarouselExtra* extra = (CarouselExtra *) carousel->UIExtra;
    extra->Acolor = Acolor;
    extra->DAcolor = DAcolor;
}

// Slider creation
UIElement* createSlider(Pos pos, char* text, int curValue, int minValue, int maxValue, int stepValue, float fontScale, float sliderOffset)
{
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

    sliderConfig->pos = (Pos)
    {
        x, y
    };

    sliderConfig->sliderOffset = sliderOffset;
    sliderConfig->defaultValue = curValue;
    sliderConfig->curValue = curValue;
    sliderConfig->minValue = minValue;
    sliderConfig->maxValue = maxValue;
    sliderConfig->stepValue = stepValue;
    sliderConfig->Acolor = COLOR_GRAY;
    sliderConfig->DAcolor = COLOR_CYAN;

    slider->UIExtra = (void *) sliderConfig;

    return slider;
}

void configureSliderColor(UIElement* slider, Color Acolor, Color DAcolor)
{
    SliderExtra* extra = (SliderExtra *) slider->UIExtra;
    extra->Acolor = Acolor;
    extra->DAcolor = DAcolor;
}


void renderMenu(Menu* menu)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    for (int i = 0; i < menu->num_elements; i++)
    {
        // Button
        if(menu->uiElements[i]->type == UI_BUTTON)
        {
            ButtonExtra* extra = (ButtonExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            {
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);
        }
        // Input field
        else if(menu->uiElements[i]->type == UI_INPUTFIELD)
        {
            InputFieldExtra* extra = (InputFieldExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            {
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
            if (extra->masking)
            {
                maskString(extra->input, output, PASSWORDMASK_CHAR);
                renderString(_x, extra->pos.y, output, extra->fontScale, extra->DAcolor);
            }
            else
            {
                renderString(_x, extra->pos.y, extra->input, extra->fontScale, extra->DAcolor);
            }

        }
        // Carousel
        else if(menu->uiElements[i]->type == UI_CAROUSEL)
        {
            CarouselExtra* extra = (CarouselExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            {
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->Acolor);
                renderString(extra->pos.x-25, extra->pos.y, ">", extra->fontScale, COLOR_GRAY);
            }
            else
                renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->DAcolor);
            renderString(extra->pos.x+140, extra->pos.y, "<", extra->fontScale, extra->Acolor);
            renderString(extra->pos.x+170, extra->pos.y, extra->options[extra->curOption], extra->fontScale, extra->DAcolor);
            renderString(extra->pos.x+290, extra->pos.y, ">", extra->fontScale, extra->Acolor);


        }
        // Slider
        else if(menu->uiElements[i]->type == UI_SLIDER)
        {
            SliderExtra* extra = (SliderExtra *) menu->uiElements[i]->UIExtra;
            if (menu->hover_element == i)
            {
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
        else if(menu->uiElements[i]->type == UI_LABEL)
        {
            LabelExtra* extra = (LabelExtra *) menu->uiElements[i]->UIExtra;
            renderString(extra->pos.x, extra->pos.y, extra->label, extra->fontScale, extra->color);
        }
    }
}

void menuBasicHandleKeyboard(Menu* menu, unsigned char key)
{
    // key == 8 -> backspace
    if (isprint(key) || key == 8)
    {
        if (menu->hover_element >= 0 && menu->hover_element <= menu->num_elements-1)
            if (menu->uiElements[menu->hover_element]->type == UI_INPUTFIELD)
            {
                InputFieldExtra* extra = (InputFieldExtra *) menu->uiElements[menu->hover_element]->UIExtra;
                int len = strlen(extra->input);
                // Backspace
                if (key == 8)
                {
                    extra->input[len-1] = '\0';
                }
                // Other characters
                else
                {
                    if (len < extra->maxLength)
                    {
                        extra->input[len] = key;
                        extra->input[len+1] = '\0';
                    }
                }
                // Automatic box resizing
                extra->boxWidth = INPUTBOX_RIGHTMARGIN+calculateTextWidth(extra->input, extra->fontScale);
                if (extra->masking)
                {
                    char output[MAX_STR_SIZE];
                    maskString(extra->input, output, PASSWORDMASK_CHAR);
                    extra->boxWidth = INPUTBOX_RIGHTMARGIN+calculateTextWidth(output, extra->fontScale);
                }
            }
    }
}

void menuBasicHandleSKeyboard(Menu* menu, int key)
{
    if (key == GLUT_KEY_DOWN)
    {
        if (menu->hover_element < menu->num_interactable_elements-1) menu->hover_element += 1;
        else menu->hover_element = 0;
    }
    else if (key == GLUT_KEY_UP)
    {
        if (menu->hover_element > 0) menu->hover_element -= 1;
        else menu->hover_element = menu->num_interactable_elements-1;
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        if (menu->hover_element >= 0 && menu->hover_element <= menu->num_elements-1)
        {
            if (menu->uiElements[menu->hover_element]->type == UI_CAROUSEL)
            {
                CarouselExtra* extra = (CarouselExtra *) menu->uiElements[menu->hover_element]->UIExtra;
                if (extra->curOption < extra->num_options-1) extra->curOption += 1;
                else extra->curOption = 0;
            }
            else if (menu->uiElements[menu->hover_element]->type == UI_SLIDER)
            {
                SliderExtra* extra = (SliderExtra *) menu->uiElements[menu->hover_element]->UIExtra;
                if (extra->curValue < extra->maxValue) extra->curValue += extra->stepValue;
            }
        }
    }

    else if (key == GLUT_KEY_LEFT)
    {
        if (menu->hover_element >= 0 && menu->hover_element <= menu->num_elements-1)
        {
            if (menu->uiElements[menu->hover_element]->type == UI_CAROUSEL)
            {
                CarouselExtra* extra = (CarouselExtra *) menu->uiElements[menu->hover_element]->UIExtra;
                if (extra->curOption > 0) extra->curOption -= 1;
                else extra->curOption = extra->num_options-1;
            }
            else if (menu->uiElements[menu->hover_element]->type == UI_SLIDER)
            {
                SliderExtra* extra = (SliderExtra *) menu->uiElements[menu->hover_element]->UIExtra;
                if (extra->curValue > extra->minValue) extra->curValue -= extra->stepValue;
            }
        }
    }
}

void resetMenuFields(Menu* menu){
    for (int i = 0; i < menu->num_interactable_elements; i++){
        // Input field
        if(menu->uiElements[i]->type == UI_INPUTFIELD)
        {
            InputFieldExtra* extra = (InputFieldExtra *) menu->uiElements[i]->UIExtra;
            strcpy(extra->input, "");
            extra->boxWidth = extra->defaultBoxWidth;

        }
        // Carousel
        else if(menu->uiElements[i]->type == UI_CAROUSEL)
        {
            CarouselExtra* extra = (CarouselExtra *) menu->uiElements[i]->UIExtra;
            extra->curOption = extra->defaultOption;
        }
        // Slider
        else if(menu->uiElements[i]->type == UI_SLIDER)
        {
            SliderExtra* extra = (SliderExtra *) menu->uiElements[i]->UIExtra;
            extra->curValue = extra->defaultValue;

        }
    }
}
