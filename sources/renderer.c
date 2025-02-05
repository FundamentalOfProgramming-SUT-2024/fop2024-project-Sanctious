#include "renderlibs.h"
#include <stdio.h>
#include <stdint.h>

#include "renderer.h"
#include "map.h"
#include "config.h"
#include "logger.h"
#include "player.h"
#include "main.h"

FT_Library ft;
FT_Face face;

uint32_t decodeUTF8(const char** text) {
    const unsigned char* ptr = (const unsigned char*)(*text);
    uint32_t code = 0;

    if (*ptr < 0x80) { // Single-byte (ASCII)
        code = *ptr;
        (*text)++;
    } else if ((*ptr & 0xE0) == 0xC0) {
        if ((*(ptr + 1) & 0xC0) != 0x80) return 0;
        code = ((*ptr & 0x1F) << 6) | (*(ptr + 1) & 0x3F);
        (*text) += 2;
    } else if ((*ptr & 0xF0) == 0xE0) {
        if ((*(ptr + 1) & 0xC0) != 0x80 || (*(ptr + 2) & 0xC0) != 0x80) return 0;
        code = ((*ptr & 0x0F) << 12) | ((*(ptr + 1) & 0x3F) << 6) | (*(ptr + 2) & 0x3F);
        (*text) += 3;
    } else if ((*ptr & 0xF8) == 0xF0) {
        if ((*(ptr + 1) & 0xC0) != 0x80 || (*(ptr + 2) & 0xC0) != 0x80 || (*(ptr + 3) & 0xC0) != 0x80) return 0;
        code = ((*ptr & 0x07) << 18) | ((*(ptr + 1) & 0x3F) << 12) | ((*(ptr + 2) & 0x3F) << 6) | (*(ptr + 3) & 0x3F);
        (*text) += 4;
    } else {
        return 0;
    }

    return code;
}

void initFreeType(const char* fontPath) {
    if (FT_Init_FreeType(&ft)) {
        Log("FreeType init failed.", _ERROR_);
        exit(1);
    }

    if (FT_New_Face(ft, fontPath, 0, &face)) {
        Log("Font couldn't be loaded.", _ERROR_);
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, FONTQUALITY, FONTQUALITY);
}

float gridCellWidth() {
    return (float)WINDOW_WIDTH / XCELLS;
}

float gridCellHeight() {
    return (float)WINDOW_HEIGHT / YCELLS;
}

void renderCell(int col, int row, const char* text, Color color, int scaleToFit) {
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float x = col * cellWidth + XBUFFER_ZONE;
    float y = row * cellHeight + YBUFFER_ZONE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(color.r, color.g, color.b, color.a);

    glEnable(GL_TEXTURE_2D);

    const char* p = text;
    while (*p) {
        uint32_t charCode = decodeUTF8(&p);

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            Log("Failed to load glyph for codepoint %u\n", _ERROR_, charCode);
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
//        float xpos = x + glyph->bitmap_left * fontScale;
//        float ypos = y - glyph->bitmap_top * fontScale;
//        float w = glyph->bitmap.width * fontScale;
//        float h = glyph->bitmap.rows * fontScale;
        float xpos1, xpos2, ypos1, ypos2;
        if (scaleToFit){
            xpos1 = x;
            xpos2 = x+cellWidth;
            ypos1 = y;
            ypos2 = y+cellHeight;
        }
        else{
            xpos1 = x + cellWidth/2 - glyph->bitmap.width*cellWidth/(2*FONTQUALITY);
            xpos2 = x + cellWidth/2 + glyph->bitmap.width*cellWidth/(2*FONTQUALITY);
            ypos1 = y + cellHeight/2 - glyph->bitmap.rows*cellHeight/(2*FONTQUALITY);
            ypos2 = y + cellHeight/2 + glyph->bitmap.rows*cellHeight/(2*FONTQUALITY);
        }

        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(xpos1, ypos1);
        glTexCoord2f(1.0, 0.0); glVertex2f(xpos2, ypos1);
        glTexCoord2f(1.0, 1.0); glVertex2f(xpos2, ypos2);
        glTexCoord2f(0.0, 1.0); glVertex2f(xpos1, ypos2);
        glEnd();

        x += cellWidth;

        glDeleteTextures(1, &texture);
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
void renderString(int _x, int _y, char* text, float fontScale, Color color) {
    int x = _x;
    int y = _y;
    //
    int maxTop = 0;
    int minBottom = 0;
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(color.r, color.g, color.b, color.a);

    glEnable(GL_TEXTURE_2D);

    const char* p = text;
    while (*p) {
        uint32_t charCode = decodeUTF8(&p);

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL)) {
            Log("Failed to load glyph for codepoint %u\n", _ERROR_, charCode);
            continue;
        }

        // Handle next line character
        if (charCode == '\n'){
            x = _x;
            y += maxTop - minBottom + LINE_INDENTATION;
            maxTop = 0;
            minBottom = 0;
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

        //
        int top = glyph->bitmap_top * fontScale;
        int bottom = top - (glyph->bitmap.rows * fontScale);
        if (top > maxTop) maxTop = top;
        if (bottom < minBottom) minBottom = bottom;
        //

        float xpos = x + glyph->bitmap_left * fontScale;
        float ypos = y - glyph->bitmap_top * fontScale;
        float w = glyph->bitmap.width * fontScale;
        float h = glyph->bitmap.rows * fontScale;

        // Render Quad with that texture
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos);         // Bottom-left
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos);    // Bottom-right
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos + h); // Top-right
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos + h);    // Top-left
        glEnd();

        x += (glyph->advance.x >> 6) * fontScale;

        glDeleteTextures(1, &texture);
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void renderQuad(Pos sp, Pos ep, Color color){
    glColor4f(color.r, color.g, color.b, color.a);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glVertex2f(sp.x, sp.y);
        glVertex2f(ep.x, sp.y);
        glVertex2f(ep.x, ep.y);
        glVertex2f(sp.x, ep.y);
    glEnd();
}

void fpsLimit(int value) {
    glutPostRedisplay();
    glutTimerFunc(1000/60, fpsLimit, 0);
}


void initializeRenderer(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(RWINDOW_WIDTH, RWINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_XPOS, WINDOW_YPOS);
    glutCreateWindow("Rogue");
    Log("Game window created.", _DEBUG_);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, RWINDOW_WIDTH, RWINDOW_HEIGHT, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initFreeType("fonts/Orbitron-Regular.ttf");
    Log("Font loaded.", _DEBUG_);

    glutKeyboardFunc(*getGameInstance()->currentScene->onKeypress);
    glutSpecialFunc(*getGameInstance()->currentScene->onSpecialKeypress);

//    glutIdleFunc(*getGameInstance()->currentScene->update);
    glutDisplayFunc(*getGameInstance()->currentScene->update);
    glutTimerFunc(0, fpsLimit, 0);
}
