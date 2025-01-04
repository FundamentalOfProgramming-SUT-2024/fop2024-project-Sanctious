#ifndef RENDERER_H
#define RENDERER_H

void glutinit(int, char**);
void renderText(int col, int row, const char* text, float textR, float textG, float textB, float alpha);
void renderString(int x, int y, char* text, float scale, float _red, float _green, float _blue, float _alpha);
float gridCellWidth();
float gridCellHeight();

#endif
