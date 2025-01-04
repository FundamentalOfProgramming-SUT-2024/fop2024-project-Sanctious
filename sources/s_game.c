#include "scene.h"
#include "renderlibs.h"
#include "renderer.h"
#include "config.h"
#include "player.h"

float r,g=1.0f,b=1.0f;

static void renderRoomBox(int x, int y, int width, int height, float r, float g, float b) {
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

static void renderPlayer(){
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "O", r, g, b, 1.0);
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "*", r, g, b, 1.0);
}

static void processKeyboard(unsigned char key, int x, int y) {
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

static void processSKeyboard(int key, int x, int y) {

	if (key == GLUT_KEY_F5){
        glutLeaveGameMode();
        exit(0);
	}
	if (key == GLUT_KEY_F1){
        changeScene(getSceneByID("main_menu"));
	}
}

static void renderDoors(Room* room){
    for (int i = 0; i < room->num_doors; i++){
        renderText(room->doors[i]->pos.gridX, room->doors[i]->pos.gridY, "+", 0.4, 0.1, 0.7, 1.0);
    }
}

static void renderCorridors(Map* map){

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            renderText(cor->path[j].gridX, cor->path[j].gridY, "O", 0.6, 0.4, 0.7, 1.0);
        }
    }
}

static void render() {
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
    renderString(0, 20, "Hello World!", FONTNORMALSCALE, 0.5f, 0.1f, 0.9f, 1.0f);

    glFlush();
}

static void playerChangeColor(int c){
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


void initscene_game(){
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    strcpy(scene->sceneID, "game");

    scene->onEnter = NULL;
    scene->onExit = NULL;
    scene->onKeypress = processKeyboard;
    scene->onSpecialKeypress = processSKeyboard;
    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
