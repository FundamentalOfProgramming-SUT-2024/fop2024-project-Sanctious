#include "../scene.h"
#include "../renderlibs.h"
#include "../renderer.h"
#include "../config.h"
#include "../player.h"
#include "../map.h"
#include "../savesystem.h"
#include "../uiutils.h"

static Menu inventoryMenu;
static Menu pauseMenu;

float r=1.0f,g=1.0f,b=1.0f;

static void renderRoomBox(float x, float y, int width, int height, Color tileColor, Color wallColor) {
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float xpos = (x-0.5) * cellWidth + XBUFFER_ZONE;
    float ypos = (y-0.5) * cellHeight + YBUFFER_ZONE;
    float w = (width+1) * cellWidth;
    float h = (height+1) * cellHeight;

    renderQuad(( Pos ){xpos, ypos}, ( Pos ){xpos+w, ypos+h}, wallColor);

    cellWidth = gridCellWidth();
    cellHeight = gridCellHeight();

    xpos = x * cellWidth + XBUFFER_ZONE;
    ypos = y * cellHeight + YBUFFER_ZONE;
    w = width * cellWidth;
    h = height * cellHeight;

    renderQuad(( Pos ){xpos, ypos}, ( Pos ){xpos+w, ypos+h}, tileColor);

}

static void renderPlayer(){
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "O", COLOR_HOT_PINK);
    renderText(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "*", COLOR_HOT_PINK);
}

static void processKeyboard(unsigned char key, int x, int y) {
    if (inventoryMenu.enabled)
        menuBasicHandleKeyboard(&inventoryMenu, key);
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
    if (key == 'i')
        inventoryMenu.enabled = inventoryMenu.enabled^1;
    // Escape key
    if (key == 27){
        saveGame();
        changeScene(getSceneByID("main_menu"));
    }

    if (key == 'g'){
        changeFloor(getCurFloor()+1);
    }
}

static void processSKeyboard(int key, int x, int y) {
    if (inventoryMenu.enabled)
        menuBasicHandleSKeyboard(&inventoryMenu, key);

	if (key == GLUT_KEY_F5){
        glutLeaveGameMode();
        exit(0);
	}
	if (key == GLUT_KEY_F1){
        changeScene(getSceneByID("registeruser_menu"));
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

static void updateInventoryMenu(){
    Player* player = getPlayerInstance();

    for (int i = 0; i < player->inventory_size; i++){
        inventoryMenu.uiElements[i] = createButton((Pos) {50, 100+i*35}, player->inventory[i]->name, FONTNORMALSCALE);
//        ((InputFieldExtra *) inventoryMenu.uiElements[0]->UIExtra)->maxLength = 20;
//        configureInputFieldColor(inventoryMenu.uiElements[0], COLOR_GRAY, COLOR_CYAN);
        configureButtonColor(inventoryMenu.uiElements[i], COLOR_AMBER, COLOR_CYAN);

    }

    inventoryMenu.num_elements = player->inventory_size;
    inventoryMenu.num_interactable_elements = player->inventory_size;


}

void removeItem(Room* room, int a){
    for (int i = a; i < room->num_items-1; i++){
        room->items[i] = room->items[i+1];
    }
    room->num_items--;

}

void pickUpItems(){
    Map* floor = getFloor(getCurFloor());
    Player* player = getPlayerInstance();
    for (int i = 0; i < floor->num_rooms; i++){
        Room* room = floor->rooms[i];

        for (int j = 0; j < room->num_items; j++){
            Item* item = room->items[j];

            if (item->pos.gridX == player->pos.gridX &&
                item->pos.gridY == player->pos.gridY){
                player->inventory[player->inventory_size++] = item;
                removeItem(room, j);
            }
        }
    }
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getFloor(getCurFloor());
    renderCorridors(map);
    pickUpItems();

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];

        Color wallsColor;
        if (room->type == RT_REGULAR) wallsColor = (Color) {0.2, 0.2, 1, 1};
        if (room->type == RT_ENCHANT) wallsColor = COLOR_PURPLE;
        if (room->type == RT_NIGHTMARE) wallsColor = COLOR_ELECTRIC_BLUE;
        if (room->type == RT_TREASURE) wallsColor = COLOR_GOLD;

        renderRoomBox(room->pos.gridX, room->pos.gridY, room->scale.gridW, room->scale.gridH,
            (Color) {0.5, 0.5, 0.5, 1}, wallsColor);

        renderDoors(room);
        for (int j = 0; j < room->num_structures; j++){
            renderText(room->structures[j]->pos.gridX, room->structures[j]->pos.gridY, "@", COLOR_BROWN);
        }

        for (int j = 0; j < room->num_items; j++){
            Item* item = room->items[j];
            char temp[5];
            sprintf(temp, "%c", item->sprite);
            renderText(item->pos.gridX, item->pos.gridY, temp, COLOR_GREEN);
        }
        for (int j = 0; j < room->scale.gridW; j++) {
            for (int k = 0; k < room->scale.gridH; k++) {
//                renderText(room->pos.gridX + j, room->pos.gridY + k, "A", 1.0, 0.0, 0.0, 1.0);
            }
        }
    }

    for (int i = 0; i < map->num_entities; i++){
        Entity* entity = map->entities[i];
        char temp[5];
        sprintf(temp, "%c", entity->sprite);
        renderText(entity->pos.gridX, entity->pos.gridY, temp, COLOR_LAVENDER);
    }
    renderPlayer();


    char message[100];
    Player* player = getPlayerInstance();
    sprintf(message, "X= %d Y= %d\nFloor= %d", player->pos.gridX, player->pos.gridY, getCurFloor());
    renderString(0, 20, message, FONTNORMALSCALE, COLOR_PURPLE);

    if (inventoryMenu.enabled){
//        glClearColor(1, 1, 1, 1);
//        glClear(GL_COLOR_BUFFER_BIT);
        updateInventoryMenu();
        renderQuad(( Pos ){50, 50}, ( Pos ){RWINDOW_WIDTH-50, RWINDOW_HEIGHT-50}, (Color) {0.5f, 0.5f, 0.5f, 0.9f} );
        renderMenu(&inventoryMenu);
    }

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
    // Inventory
    inventoryMenu.enabled = 0;
    inventoryMenu.num_elements = 0;
    inventoryMenu.num_interactable_elements = 0;

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
