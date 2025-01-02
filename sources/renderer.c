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
#include "player.h"

FT_Library ft;
FT_Face face;

float r,g=1.0f,b=1.0f;

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
    glColor3f(0.2, 0.2, 1);

    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float xpos = (x-1) * cellWidth + XBUFFER_ZONE;
    float ypos = (y-1) * cellHeight + YBUFFER_ZONE;
    float w = (width+2) * cellWidth;
    float h = (height+2) * cellHeight;

    glDisable(GL_BLEND);

    glBegin(GL_QUADS);
    glVertex2f(xpos, ypos);
    glVertex2f(xpos + w, ypos);
    glVertex2f(xpos + w, ypos + h);
    glVertex2f(xpos, ypos + h);
    glEnd();

    glColor3f(r, g, b);

    cellWidth = gridCellWidth();
    cellHeight = gridCellHeight();

    xpos = x * cellWidth + XBUFFER_ZONE;
    ypos = y * cellHeight + YBUFFER_ZONE;
    w = width * cellWidth;
    h = height * cellHeight;

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

    float x = col * cellWidth + XBUFFER_ZONE;
    float y = row * cellHeight + YBUFFER_ZONE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(textR, textG, textB, alpha);

    glEnable(GL_TEXTURE_2D);

    for (const char* p = text; *p; p++) {
        unsigned int charCode = *p;

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
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
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(_red, _green, _blue, _alpha);

    glEnable(GL_TEXTURE_2D);

    for (const char* p = text; *p; p++) {
        unsigned int charCode = *p;

        if (FT_Load_Char(face, charCode, FT_LOAD_RENDER)) {
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

void renderPlayer(){
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "O", r, g, b, 1.0);
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "*", r, g, b, 1.0);
}

void processKeyboard(unsigned char key, int x, int y) {
    Player* player = getPlayerInstance();

	if (key == 'w')
        if (isValidPos(player->pos.gridX, player->pos.gridY-1))
            player->pos.gridY -= 1;
    if (key == 'd')
        if (isValidPos(player->pos.gridX+1, player->pos.gridY))
            player->pos.gridX += 1;
    if (key == 's')
        if (isValidPos(player->pos.gridX, player->pos.gridY+1))
            player->pos.gridY += 1;
    if (key == 'a')
        if (isValidPos(player->pos.gridX-1, player->pos.gridY))
            player->pos.gridX -= 1;
}

void processSKeyboard(int key, int x, int y) {
    Player* player = getPlayerInstance();

	if (key == GLUT_KEY_F5){
        glutLeaveGameMode();
        exit(0);
	}
}

void renderDoors(Room* room){
    for (int i = 0; i < room->num_doors; i++){
        renderText(room->doors[i]->pos.gridX, room->doors[i]->pos.gridY, "+", 0.4, 0.1, 0.7, 1.0);
    }
}

void renderCorridors(Map* map){

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            renderText(cor->path[j].gridX, cor->path[j].gridY, "O", 0.6, 0.4, 0.7, 1.0);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getMapInstance();

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];

        renderRoomBox(room->pos.gridX, room->pos.gridY, room->scale.gridW, room->scale.gridH, 0.5, 0.5, 0.5);
        renderDoors(room);

        for (int j = 0; j < room->scale.gridW; j++) {
            for (int k = 0; k < room->scale.gridH; k++) {
//                renderText(room->pos.gridX + j, room->pos.gridY + k, "A", 1.0, 0.0, 0.0, 1.0);
            }
        }
    }

    renderCorridors(map);
    renderPlayer();
    renderString(0, 20, "Hello World!", 0.5f, 0.1f, 0.9f, 1.0f);

    glFlush();
}

void playerChangeColor(int c){
    if (c%2){
        r = 1.0f;
        g = 1.0f;
        b = 0.0f;
    }
    else{
        r = 0.0f;
        b = 1.0f;
        g = 1.0f;
    }
    glutTimerFunc(500, playerChangeColor, c+1);
}

void render(int argc, char** argv) {
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

    glutKeyboardFunc(processKeyboard);
    glutSpecialFunc(processSKeyboard);

    glutIdleFunc(display);
    glutDisplayFunc(display);

    glutTimerFunc(500, playerChangeColor, 0);

    Log("Entering game loop...", _DEBUG_);
    glutMainLoop();
}
