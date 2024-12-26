#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <ft2build.h>
#include <stdio.h>
#include FT_FREETYPE_H

#include "renderer.h"
#include "map.h"
#include "config.h"
#include "logger.h"

FT_Library ft;
FT_Face face;

void initFreeType(const char* fontPath) {
    if (FT_Init_FreeType(&ft)) {
        Log("FreeType init failed.", _ERROR_);
        exit(1);
    }

    if (FT_New_Face(ft, fontPath, 0, &face)) {
        Log("Font couldn't be loaded.", _ERROR_);
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
}

float gridCellWidth() {
    return (float)WINDOW_WIDTH / XCELLS;
}

float gridCellHeight() {
    return (float)WINDOW_HEIGHT / YCELLS;
}

void renderRoomBox(int x, int y, int width, int height, float r, float g, float b) {
    glColor3f(r, g, b);

    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float xpos = x * cellWidth;
    float ypos = y * cellHeight;
    float w = width * cellWidth;
    float h = height * cellHeight;

    glDisable(GL_BLEND);

    glBegin(GL_QUADS);
    glVertex2f(xpos, ypos);
    glVertex2f(xpos + w, ypos);
    glVertex2f(xpos + w, ypos + h);
    glVertex2f(xpos, ypos + h);
    glEnd();
}

void renderText(int col, int row, const char* text, float textR, float textG, float textB, float alpha) {
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float x = col * cellWidth;
    float y = row * cellHeight;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(textR, textG, textB, alpha);

    glEnable(GL_TEXTURE_2D);

    for (const char* p = text; *p; p++) {
        unsigned int charCode = *p;

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load Glyph for '%c'\n", *p);
            continue;
        }

        FT_GlyphSlot glyph = face->glyph;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_ALPHA,
            glyph->bitmap.width,
            glyph->bitmap.rows,
            0,
            GL_ALPHA,
            GL_UNSIGNED_BYTE,
            glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//        Log("glyph xsize: %d xpos: %f ysize: %d", _DEBUG_, glyph->bitmap.width, x, glyph->bitmap.rows);

        // Render quad with that texture ( glyph texture )
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(x, y);
        glTexCoord2f(1.0, 0.0); glVertex2f(x + cellWidth, y);
        glTexCoord2f(1.0, 1.0); glVertex2f(x + cellWidth, y + cellHeight);
        glTexCoord2f(0.0, 1.0); glVertex2f(x, y + cellHeight);
        glEnd();

        x += cellWidth;

        glDeleteTextures(1, &texture);
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getMapInstance();

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];

        renderRoomBox(room->gridXPosition, room->gridYPosition, room->gridWidth, room->gridHeight, 1.0, 1.0, 1.0);

        for (int j = 0; j < room->gridWidth; j++) {
            for (int k = 0; k < room->gridHeight; k++) {
                renderText(room->gridXPosition + j, room->gridYPosition + k, "A", 1.0, 0.0, 0.0, 1.0);
            }
        }
    }

    glFlush();
}

void render(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_XPOS, WINDOW_YPOS);
    glutCreateWindow("Rogue");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initFreeType("fonts/FreeSans.otf");

    glutDisplayFunc(display);

    glutMainLoop();
}
