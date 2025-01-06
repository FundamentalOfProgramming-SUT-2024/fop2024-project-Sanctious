#include "../scene.h"
#include "../renderlibs.h"
#include "../renderer.h"
#include "../config.h"
#include "../player.h"

float r=1.0f,g=1.0f,b=1.0f;

static void renderRoomBox(float x, float y, int width, int height, Color tileColor, Color wallColor) {
    glColor3f(wallColor.r, wallColor.g, wallColor.b);

    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float xpos = (x-0.5) * cellWidth + XBUFFER_ZONE;
    float ypos = (y-0.5) * cellHeight + YBUFFER_ZONE;
    float w = (width+1) * cellWidth;
    float h = (height+1) * cellHeight;

    glDisable(GL_BLEND);

    glBegin(GL_QUADS);
    glVertex2f(xpos, ypos);
    glVertex2f(xpos + w, ypos);
    glVertex2f(xpos + w, ypos + h);
    glVertex2f(xpos, ypos + h);
    glEnd();

    glColor3f(tileColor.r, tileColor.g, tileColor.b);

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
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "O", COLOR_HOT_PINK);
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "*", COLOR_HOT_PINK);
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
        renderText(room->doors[i]->pos.gridX, room->doors[i]->pos.gridY, "+", COLOR_BROWN);
    }
}

static void renderCorridors(Map* map){

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            Color color = COLOR_AMBER;
            color.a = 0.5f;
            renderText(cor->path[j].gridX, cor->path[j].gridY, "ô", color);
        }
    }
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getMapInstance();
    renderCorridors(map);

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];

        Color wallsColor;
        if (room->type == REGULAR) wallsColor = (Color) {0.2, 0.2, 1, 1};
        if (room->type == ENCHANT) wallsColor = COLOR_PURPLE;
        if (room->type == NIGHTMARE) wallsColor = COLOR_ELECTRIC_BLUE;
        if (room->type == TREASURE) wallsColor = COLOR_GOLD;

        renderRoomBox(room->pos.gridX, room->pos.gridY, room->scale.gridW, room->scale.gridH,
            (Color) {0.5, 0.5, 0.5, 1}, wallsColor);

        renderDoors(room);
        for (int j = 0; j < room->num_structures; j++){
            renderText(room->structures[j]->pos.gridX, room->structures[j]->pos.gridY, "@", COLOR_BROWN);
        }
        for (int j = 0; j < room->scale.gridW; j++) {
            for (int k = 0; k < room->scale.gridH; k++) {
//                renderText(room->pos.gridX + j, room->pos.gridY + k, "A", 1.0, 0.0, 0.0, 1.0);
            }
        }
    }

    renderPlayer();
    char message[100];
    Player* player = getPlayerInstance();
    sprintf(message, "X= %d Y= %d", player->pos.gridX, player->pos.gridY);
    renderString(0, 20, message, FONTNORMALSCALE, COLOR_PURPLE);

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
