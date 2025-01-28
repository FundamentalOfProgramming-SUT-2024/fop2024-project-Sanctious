#ifndef RENDERER_H
#define RENDERER_H

typedef struct{
    float r, g, b, a;
} Color;

typedef struct{
    float x, y;
} Pos;

typedef struct{
    float w, h;
} Scale;

void initializeRenderer(int, char**);
void renderCell(int col, int row, const char* text, Color colorl, int scaleToFit);
void renderString(int x, int y, char* text, float fontScale, Color color);
float gridCellWidth();
float gridCellHeight();
void renderQuad(Pos sp, Pos ep, Color color);

#endif
