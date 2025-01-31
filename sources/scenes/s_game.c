#include "../scene.h"
#include "../renderlibs.h"
#include "../renderer.h"
#include "../config.h"
#include "../player.h"
#include "../map.h"
#include "../savesystem.h"
#include "../uiutils.h"

static int curMenu = 0;
static int invtabs_c = 4;
static Menu invtabs[4];
static Menu pauseMenu;


static void pickUpItems();

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
void playerAction(){
    getCurrentSave()->gametime++;
    Map* map = getFloor(getCurFloor());
    for (int i = 0; i < map->num_entities; i++){
        EntityOnAction(map->entities[i]);
    }


}

static void processKeyboard(unsigned char key, int x, int y) {
    // Number system for item types to make it easier to do other stuff ( automate stuff )

    if (key == 'i'){
        for (int i = 0; i < invtabs_c; i++){
            invtabs[i].enabled = invtabs[i].enabled^1;
        }
    }

    if (invtabs[0].enabled){
        menuBasicHandleKeyboard(&invtabs[curMenu], key);
        // Enter key
        if (key == 13){
//            if (inventoryMenu.hover_element >= 0 && inventoryMenu.hover_element <= inventoryMenu.num_elements-1){
////                getPlayerInstance()->inventory[inventoryMenu->hover_element];
//
//            }

        }
        if (key == 'q'){
            Player* player = getPlayerInstance();
            InvSlotExtra* extra = ((InvSlotExtra *) (invtabs[curMenu].uiElements[invtabs[curMenu].hover_element]->UIExtra));
            removeItemFromPlayer(player, extra->itemIndex);
            addItemToRoom(findPlayerRoom(), extra->item);
            extra->item->pos = player->pos;
        }

        if (key == 'e'){
            Player* player = getPlayerInstance();
            InvSlotExtra* extra = ((InvSlotExtra *) (invtabs[curMenu].uiElements[invtabs[curMenu].hover_element]->UIExtra));

            if (ItemOnConsume(extra->item)){
                removeItemFromPlayer(player, extra->itemIndex);
            }
        }
        return;
    }
    if (key == 'e'){
        pickUpItems();
    }

    Player* player = getPlayerInstance();
	if (key == 'w')
        if (isValidPos(player->pos.gridX, player->pos.gridY-1)){
            player->pos.gridY -= 1;
            playerAction();
        }
    if (key == 'd')
        if (isValidPos(player->pos.gridX+1, player->pos.gridY)){
            player->pos.gridX += 1;
            playerAction();
        }
    if (key == 's')
        if (isValidPos(player->pos.gridX, player->pos.gridY+1)){
            player->pos.gridY += 1;
            playerAction();
        }
    if (key == 'a')
        if (isValidPos(player->pos.gridX-1, player->pos.gridY)){
            player->pos.gridX -= 1;
            playerAction();
        }
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
    if (key == GLUT_KEY_RIGHT){
        if (curMenu < invtabs_c-1){
            invtabs[curMenu].hover_element = -1;
            curMenu++;
            invtabs[curMenu].hover_element = 0;
        }
    }
    if (key == GLUT_KEY_LEFT){
        if (0 < curMenu){
            invtabs[curMenu].hover_element = -1;
            curMenu--;
            invtabs[curMenu].hover_element = 0;
        }
    }
    if (invtabs[0].enabled){
        menuBasicHandleSKeyboard(&invtabs[curMenu], key);
        return;
    }

	if (key == GLUT_KEY_F5){
        glutLeaveGameMode();
        exit(0);
	}
	if (key == GLUT_KEY_F1){
        changeScene(getSceneByID("registeruser_menu"));
	}
}

static void updateInventoryMenu(){
    Player* player = getPlayerInstance();
    for (int i = 0; i < invtabs_c; i++){
        invtabs[i].num_elements = 0;
        invtabs[i].num_interactable_elements = 0;
    }

    for (int i = 0; i < player->inventory_size; i++){
        Item* item = player->inventory[i];

        Menu* _menu = &invtabs[item->itemclass];

        int xpos = 70 + item->itemclass*200;
        int count = _menu->num_elements;

        char temp[100];
        sprintf(temp, "%s %dX", player->inventory[i]->name, player->inventory[i]->count);

        _menu->uiElements[count] = createInvSlot((Pos) {xpos, 100+count*35}, temp, FONTNORMALSCALE, item, i);
        configureInvSlotColor(_menu->uiElements[count], COLOR_AMBER, COLOR_CYAN);
        _menu->num_elements++;
        _menu->num_interactable_elements++;

//        ((InputFieldExtra *) inventoryMenu.uiElements[0]->UIExtra)->maxLength = 20;
//        configureInputFieldColor(inventoryMenu.uiElements[0], COLOR_GRAY, COLOR_CYAN);


    }

//    inventoryMenu.num_elements = player->inventory_size;
//    inventoryMenu.num_interactable_elements = player->inventory_size;


}



static void pickUpItems(){
    Map* floor = getFloor(getCurFloor());
    Player* player = getPlayerInstance();
    for (int i = 0; i < floor->num_rooms; i++){
        Room* room = floor->rooms[i];

        for (int j = 0; j < room->num_items; j++){
            Item* item = room->items[j];

            if (item->pos.gridX == player->pos.gridX &&
                item->pos.gridY == player->pos.gridY){
                addItemToPlayer(player, item);
                removeItemFromRoom(room, j);
                break;
            }
        }
    }
}

static void renderPlayer(Player* player){
//    renderCell(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "ò", COLOR_HOT_PINK, 0);
    renderCell(player->pos.gridX, player->pos.gridY, "ð", COLOR_HOT_PINK, 0);
//    renderCell(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "i", COLOR_HOT_PINK, 0);
//    renderCell(getPlayerInstance()->pos.gridX, getPlayerInstance()->pos.gridY, "*", COLOR_HOT_PINK, 0);
}

static void renderPlayerStatus(Player* player){
    char message[100];
    sprintf(message, "\u0100 \u0101 \u0102 \u0103 \u0104Hunger: %d", player->health,
            player->armor, player->gold, player->hunger);
    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
}


static void renderRoomDoors(Room* room){
    for (int i = 0; i < room->num_doors; i++){
        renderCell(room->doors[i]->pos.gridX, room->doors[i]->pos.gridY, "+", COLOR_BROWN, 1);
    }
}

static void renderCorridors(Map* map){

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            Color color = COLOR_AMBER;
            color.a = 0.5f;
            renderCell(cor->path[j].gridX, cor->path[j].gridY, "ô", color, 1);
        }
    }
}

static void renderRoomItems(Room* room){
    for (int j = 0; j < room->num_items; j++){
        Item* item = room->items[j];
        renderCell(item->pos.gridX, item->pos.gridY, item->sprite, item->spriteColor, 0);
    }
}

static void renderRoomStructures(Room* room){
    for (int j = 0; j < room->num_structures; j++){
        Structure* structure = room->structures[j];
        renderCell(structure->pos.gridX, structure->pos.gridY, structure->sprite, structure->spriteColor, 0);
    }
}

static void renderEntities(Map* map){
    for (int i = 0; i < map->num_entities; i++){
        Entity* entity = map->entities[i];
        renderCell(entity->pos.gridX, entity->pos.gridY, entity->sprite, entity->spriteColor, 0);
    }
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getFloor(getCurFloor());
    renderCorridors(map);

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];

        renderRoomBox(room->pos.gridX, room->pos.gridY, room->scale.gridW, room->scale.gridH,
            room->floorsColor, room->wallsColor);
        renderRoomDoors(room);
        renderRoomItems(room);
        renderRoomStructures(room);

    }
    renderEntities(map);


    char message[100];
    Player* player = getPlayerInstance();
    sprintf(message, "X= %d Y= %d\nFloor= %d", player->pos.gridX, player->pos.gridY, getCurFloor());
    renderString(0, 20, message, FONTNORMALSCALE, COLOR_PURPLE);

    renderPlayer(player);
    renderPlayerStatus(player);

    if (invtabs[0].enabled){
//        glClearColor(1, 1, 1, 1);
//        glClear(GL_COLOR_BUFFER_BIT);
        updateInventoryMenu();
        renderQuad(( Pos ){50, 50}, ( Pos ){RWINDOW_WIDTH-50, RWINDOW_HEIGHT-50}, (Color) {0.5f, 0.5f, 0.5f, 0.9f} );
        renderString(650, 70, "-Potions", FONTNORMALSCALE, COLOR_AMETHYST);
        renderString(50, 70, "-Melee", FONTNORMALSCALE, COLOR_AMETHYST);
        renderString(250, 70, "-Ranged", FONTNORMALSCALE, COLOR_AMETHYST);
        renderString(450, 70, "-Foods", FONTNORMALSCALE, COLOR_AMETHYST);
        for (int i = 0; i < invtabs_c; i++){
            renderMenu(&invtabs[i]);
        }
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
    for (int i = 0; i < invtabs_c; i++){
//        invtabs[i] = (Menu *) malloc(1 * sizeof(Menu));
        invtabs[i].hover_element = -1;
        invtabs[i].enabled = 0;
        invtabs[i].num_elements = 0;
        invtabs[i].num_interactable_elements = 0;
    }
    invtabs[0].hover_element = 0;

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
