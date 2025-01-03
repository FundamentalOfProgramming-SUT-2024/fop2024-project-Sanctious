#include "renderlibs.h"
#include <stdio.h>

#include "renderer.h"
#include "map.h"
#include "config.h"
#include "logger.h"
#include "player.h"
#include "main.h"

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

void renderText(int col, int row, const char* text, float textR, float textG, float textB, float alpha) {
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float x = col * cellWidth + XBUFFER_ZONE;
    float y = row * cellHeight + YBUFFER_ZONE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(textR, textG, textB, alpha);

    glEnable(GL_TEXTURE_2D);

    for (const char* p = text; *p; p++) {
        unsigned int charCode = *p;

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            Log("Failed to load glyph.", _ERROR_);
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

void renderString(int x, int y, char* text, float _red, float _green, float _blue, float _alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(_red, _green, _blue, _alpha);

    glEnable(GL_TEXTURE_2D);

    for (const char* p = text; *p; p++) {
        unsigned int charCode = *p;

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            Log("Failed to load glyph.", _ERROR_);
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

        // Render quad with that texture ( glyph texture )
        float scale = 0.5f;
        float xpos = x + face->glyph->bitmap_left * scale;
        float ypos = y - (face->glyph->bitmap_top) * scale;
        float w = face->glyph->bitmap.width * scale;
        float h = face->glyph->bitmap.rows * scale;

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);         // Bottom-left
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos);    // Bottom-righ1
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos + h); // Top-right
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + h);    // Top-left
        glEnd();

        x += (face->glyph->advance.x >> 6) * scale;

        glDeleteTextures(1, &texture);
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void glutinit(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH+XBUFFER_ZONE+EXTRA_BUFFER, WINDOW_HEIGHT+YBUFFER_ZONE+EXTRA_BUFFER+LOWER_YBUFFER_ZONE);
    glutInitWindowPosition(WINDOW_XPOS, WINDOW_YPOS);
    glutCreateWindow("Rogue");
//    glutCreateWindow()
    Log("Game window created.", _DEBUG_);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH+XBUFFER_ZONE+EXTRA_BUFFER, WINDOW_HEIGHT+YBUFFER_ZONE+EXTRA_BUFFER+LOWER_YBUFFER_ZONE, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initFreeType("fonts/Orbitron-Regular.ttf");
    Log("Font loaded.", _DEBUG_);

    glutKeyboardFunc(*getGameInstance()->currentScene->onKeypress);
    glutSpecialFunc(*getGameInstance()->currentScene->onSpecialKeypress);

    glutIdleFunc(*getGameInstance()->currentScene->update);
    glutDisplayFunc(*getGameInstance()->currentScene->update);

//    glutTimerFunc(500, playerChangeColor, 0);

    Log("Entering game loop...", _DEBUG_);
    glutMainLoop();
}
