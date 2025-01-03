#include "uiutils.h"
#include "stdlib.h"
#include <strings.h>

//Color* ColorRed = NULL;
//
//void initializeColors(){
//    *ColorRed = (Color) {0.5, 0.5, 0.5, 1};
//
//}


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
