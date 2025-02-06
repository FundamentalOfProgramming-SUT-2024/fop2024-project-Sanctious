#include "../scene.h"
#include "../renderlibs.h"
#include "../renderer.h"
#include "../config.h"
#include "../player.h"
#include "../map.h"
#include "../savesystem.h"
#include "../uiutils.h"
#include "../auth.h"
#include "../utils.h"
#include "../main.h"

char eventBar[MAX_STRING_INPUT*50];

static int time = 0;
static int curMenu = 0;
static int invtabs_c = 4;
static Menu invtabs[4];
static Menu pauseMenu;

static int showMap = 0;

int shooting = 0;
Direction shootDir = NONE;
Direction prevShootDir = NONE;
//UP      = 0,
//    LEFT    = 1,
//    DOWN    = 2,
//    RIGHT

static Menu endMenu;

static void pickUpItems();
static void moveFloor(int offset);

float r=1.0f,g=1.0f,b=1.0f;

void resetEventMessage(){
    strcpy(eventBar, "");
}

void addEventMessage(char eventMsg[MAX_STRING_INPUT], ...){
    va_list args;
    char temp[MAX_STRING_INPUT];
    va_start(args, eventMsg);
    vsprintf(temp, eventMsg, args);
    va_end(args);
    strcat(temp, "\n");
    strcat(eventBar, temp);
}

void renderEventBar(){
    renderString(0, 30, eventBar, FONTNORMALSCALE*0.75, COLOR_PURPLE);
}

void revealCorridor(){
    Player* player = getPlayerInstance();
    Corridor* corridor = findPlayerCorridor();

    if (corridor != NULL){
        for (int i = 0; i < corridor->path_length; i++){
            if (distancePos(player->pos, corridor->path[i]) <= 2){
                corridor->visited[i] = 1;
            }
        }


    }

}

void revealMap(){
    if (findPlayerRoom() != NULL){
        findPlayerRoom()->visited = 1;
    }
    revealCorridor();
}

static void renderRoomBox(float x, float y, int width, int height, Color tileColor, Color wallColor) {
    float cellWidth = gridCellWidth();
    float cellHeight = gridCellHeight();

    float xpos = (x-0.4) * cellWidth + XBUFFER_ZONE;
    float ypos = (y-0.4) * cellHeight + YBUFFER_ZONE;
    float w = ((float) width + 0.8) * cellWidth;
    float h = ((float) height + 0.8) * cellHeight;

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
    int curTime = ++getCurrentSave()->gametime;
    Map* map = getFloor(getCurFloor());
    Player* player = getPlayerInstance();
    SaveInfo* saveinfo = getCurrentSave();

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        for (int j = 0; j < room->num_structures;){
            Structure* structure = room->structures[j];
            if (comparePos(structure->pos, player->pos)){
                if (StructureOnStep(structure)){
                    removeStructureFromRoom(room, j);
                }
                else{
                    j++;
                }
            }
            else{
                j++;
            }
        }
    }

    revealMap();

    for (int i = 0; i < 3; i++){
        player->multiesT[i] = clamp(player->multiesT[i]-1, 0, 1000000);

        if (player->multiesT[i] == 0){
            player->multies[i] = player->def_multies[i];
        }
    }

    // Speed
    if (curTime % player->multies[1] != 0){
        return;
    }

    player->combatTagTime++;

    if (findPlayerRoom() != NULL && findPlayerRoom()->type == RT_ENCHANT){
        modifyPlayerHealth(player, -1);
        addEventMessage("A sinister force is decreasing your health by 1\u0100");
    }

    if (curTime%saveinfo->difficulty.healthRegenCDT == 0 && player->hunger == player->maxHunger){
        modifyPlayerHealth(player, 1*player->multies[2]);
    }
    if (curTime%saveinfo->difficulty.hungerDrainTicks == 0){
        modifyPlayerHunger(player, -1);
    }

    for (int i = 0; i < map->num_entities; i++){
        if (map->entities[i]->frozen) continue;
        EntityOnAction(map->entities[i]);
    }

    if(player->health == 0){
        getCurrentSave()->gameFinished = 1;
    }

    if (player->hunger <= saveinfo->difficulty.starveThreshold){
        addEventMessage("You are starving!");
        modifyPlayerHealth(player, -1);
    }

    if (getCurFloor() == getNumFloors()-1 && getFloor(getCurFloor())->num_entities == 0){
        saveinfo->gameFinished = 1;
    }

    for (int i = 0; i < map->num_entities; i++){
        map->entities[i]->frozen = 0;
    }
}

void showEndScreen(){
    char temp[100];
    Player* player = getPlayerInstance();

    renderQuad(( Pos ){50, 50}, ( Pos ){RWINDOW_WIDTH-50, RWINDOW_HEIGHT-50}, (Color) {0.2f, 0.5f, 0.5f, 0.9f} );
    if (player->health == 0){
        renderString(70, 120, "You lost !", FONTNORMALSCALE*1.5, COLOR_CRIMSON);
    }
    else{
        renderString(70, 120, "You won !", FONTNORMALSCALE*1.5, COLOR_GREEN);
    }
    sprintf(temp, "Collected gold : %d", player->gold);
    renderString(70, 170, temp, FONTNORMALSCALE*0.9, COLOR_CRIMSON);
    sprintf(temp, "Number of items : %d", player->inventory_size);
    renderString(70, 220, temp, FONTNORMALSCALE*0.9, COLOR_CRIMSON);

    renderString(70, 270, "Press \"Enter\" to continue.", FONTNORMALSCALE, COLOR_GRAY);
//    renderMenu(&endMenu);

}

static void processKeyboard(unsigned char key, int x, int y) {
    if (shooting && shootDir == NONE){
        return;
    }

    if (key == '*'){
        getCurrentSave()->gameFinished = 1;
    }
    if (getCurrentSave()->gameFinished){
        if (key == 13){
            getCurrentSave()->gold = getPlayerInstance()->gold;
            getCurrentUser()->stats.sumGold += getPlayerInstance()->gold;
            getCurrentUser()->stats.sumScores += getPlayerInstance()->score;
            getCurrentUser()->stats.num_games += 1;
            saveGame();
            updateUser(getCurrentUser());
            changeScene(getSceneByID("endgame"));
        }
        menuBasicHandleKeyboard(&endMenu, key);
        return;
    }
    // Number system for item types to make it easier to do other stuff ( automate stuff )
    if (key == 'i'){
        for (int i = 0; i < invtabs_c; i++){
            invtabs[i].enabled = invtabs[i].enabled^1;
        }
    }

    if (invtabs[0].enabled){
        menuBasicHandleKeyboard(&invtabs[curMenu], key);
        Player* player = getPlayerInstance();
        InvSlotExtra* extra;
        if (invtabs[curMenu].num_elements != 0)
            extra = ((InvSlotExtra *) (invtabs[curMenu].uiElements[invtabs[curMenu].hover_element]->UIExtra));
        // Enter key
        if (key == 13){
//            if (inventoryMenu.hover_element >= 0 && inventoryMenu.hover_element <= inventoryMenu.num_elements-1){
////                getPlayerInstance()->inventory[inventoryMenu->hover_element];
//
//            }
            resetEventMessage();
            playerAction();
            addEventMessage("Equipped %s", extra->item->name);
            player->equippedItem = extra->item;

        }
        if (key == 'q'){
            resetEventMessage();
            playerAction();
            removeItemFromPlayer(player, extra->itemIndex);
            addItemToRoom(findPlayerRoom(), extra->item);
            extra->item->pos = player->pos;
        }

        if (key == 'e'){
            resetEventMessage();
            playerAction();
            if (ItemOnConsume(extra->item)){
                removeItemFromPlayer(player, extra->itemIndex);
            }
        }
        return;
    }
    Player* player = getPlayerInstance();
    if (key == 'e'){
        resetEventMessage();
        pickUpItems();
        moveFloor(1);
        revealMap();
        playerAction();
    }
    if (key == 'q'){
        resetEventMessage();
        moveFloor(-1);
        revealMap();
        playerAction();
    }
    if (key == 't'){
        if (player->equippedItem != NULL && player->equippedItem->itemclass == IC_RANGEDWEAPON){
            if (isInsideRoom(player->pos)){
                if (prevShootDir != NONE){
                    shooting = 1;
                    shootDir = prevShootDir;

                    resetEventMessage();
                    ItemOnAttack(getPlayerInstance()->equippedItem);
                    playerAction();

                    shootDir = NONE;
                    shooting = 0;
                }
                else{
                    addEventMessage("You haven’t shot anything yet");
                }
            }
            else{
                addEventMessage("The passageway is too tight to throw anything");
            }
        }
    }
    if (key == 'f'){
        if (player->equippedItem != NULL && player->equippedItem->itemclass == IC_RANGEDWEAPON){
            resetEventMessage();
            if (isInsideRoom(player->pos)){
                shooting = 1;
                addEventMessage("Choose the direction by using the arrow keys \u0110\u0111\u0112\u0113");
            }
            else{
                addEventMessage("The passageway is too tight to throw anything");
            }
        }
        else if (player->equippedItem != NULL){
            resetEventMessage();
            ItemOnAttack(player->equippedItem);
            playerAction();
        }
    }
    if (key == 'm'){
        showMap ^= 1;
    }
	if (key == 'w')
        if (isValidPos(player->pos.gridX, player->pos.gridY-1)){
            player->pos.gridY -= 1;
            resetEventMessage();
            playerAction();
        }
    if (key == 'd')
        if (isValidPos(player->pos.gridX+1, player->pos.gridY)){
            player->pos.gridX += 1;
            resetEventMessage();
            playerAction();
        }
    if (key == 's')
        if (isValidPos(player->pos.gridX, player->pos.gridY+1)){
            player->pos.gridY += 1;
            resetEventMessage();
            playerAction();
        }
    if (key == 'a')
        if (isValidPos(player->pos.gridX-1, player->pos.gridY)){
            player->pos.gridX -= 1;
            resetEventMessage();
            playerAction();
        }
    if (key == '1')
        if (isValidPos(player->pos.gridX+1, player->pos.gridY-1)){
            player->pos.gridX += 1;
            player->pos.gridY -= 1;
            resetEventMessage();
            playerAction();
        }
    if (key == '2')
        if (isValidPos(player->pos.gridX-1, player->pos.gridY+1)){
            player->pos.gridX -= 1;
            player->pos.gridY += 1;
            resetEventMessage();
            playerAction();
        }
    if (key == '3')
        if (isValidPos(player->pos.gridX+1, player->pos.gridY+1)){
            player->pos.gridX += 1;
            player->pos.gridY += 1;
            resetEventMessage();
            playerAction();
        }
    if (key == '4')
        if (isValidPos(player->pos.gridX-1, player->pos.gridY-1)){
            player->pos.gridX -= 1;
            player->pos.gridY -= 1;
            resetEventMessage();
            playerAction();
        }
    // Escape key
    if (key == 27){
        saveGame();
        updateUser(getCurrentUser());
        changeScene(getSceneByID("main_menu"));
    }

}

static void processSKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT && shooting){
        shootDir = RIGHT;
    }
    else if (key == GLUT_KEY_LEFT && shooting){
        shootDir = LEFT;
    }
    else if (key == GLUT_KEY_DOWN && shooting){
        shootDir = DOWN;
    }
    else if (key == GLUT_KEY_UP && shooting){
        shootDir = UP;
    }

    if (shootDir != NONE){
        resetEventMessage();
        ItemOnAttack(getPlayerInstance()->equippedItem);
        playerAction();

        prevShootDir = shootDir;
        shootDir = NONE;
        shooting = 0;
        return;
    }

    if (getCurrentSave()->gameFinished){
        menuBasicHandleSKeyboard(&endMenu, key);
        return;
    }

    if (key == GLUT_KEY_RIGHT && invtabs[0].enabled){
        if (curMenu < invtabs_c-1){
            invtabs[curMenu].hover_element = -1;
            curMenu++;
            invtabs[curMenu].hover_element = 0;
        }
    }
    if (key == GLUT_KEY_LEFT && invtabs[0].enabled){
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

        int xpos = 110 + item->itemclass*400;
        int count = _menu->num_elements;

        _menu->uiElements[count] = createInvSlot((Pos) {xpos, 150+count*35}, item->name, FONTNORMALSCALE, item, i);
        configureInvSlotColor(_menu->uiElements[count], COLOR_AMBER, COLOR_CYAN);
        _menu->num_elements++;
        _menu->num_interactable_elements++;

//        ((InputFieldExtra *) inventoryMenu.uiElements[0]->UIExtra)->maxLength = 20;
//        configureInputFieldColor(inventoryMenu.uiElements[0], COLOR_GRAY, COLOR_CYAN);


    }

    for (int i = 0; i < invtabs_c; i++){
        for (int j = 0; j < invtabs[i].num_elements; j++){
            InvSlotExtra* extra = (InvSlotExtra *) invtabs[i].uiElements[j]->UIExtra;
            Item* item = extra->item;

            char temp[100];
            sprintf(temp, "(%d x)", item->count);
            invtabs[i].uiElements[invtabs[i].num_elements+j] = createLabel((Pos) {extra->pos.x+calculateTextWidth(extra->label, FONTNORMALSCALE)+10, extra->pos.y}, temp, FONTNORMALSCALE*0.7, COLOR_CYAN);
        }
        invtabs[i].num_elements *= 2;
    }
//    inventoryMenu.num_elements = player->inventory_size;
//    inventoryMenu.num_interactable_elements = player->inventory_size;


}

static void moveFloor(int offset){
    Map* floor = getFloor(getCurFloor());
    Player* player = getPlayerInstance();
    for (int i = 0; i < floor->num_rooms; i++){
        Room* room = floor->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = room->structures[j];

            if (structure->pos.gridX == player->pos.gridX &&
                structure->pos.gridY == player->pos.gridY &&
                structure->type == ST_STAIRS){
                StairsExtra* extra = (StairsExtra *) structure->StructureExtra;
                if (offset == -1 && extra->prevPos.gridX != -1){
                    player->pos = extra->prevPos;
                    changeFloor(getCurFloor()+offset);
                    addEventMessage("Descended to floor: %d", getCurFloor()+1);
                }
                if (offset == 1 && extra->nextPos.gridX != -1){
                    player->pos = extra->nextPos;
                    changeFloor(getCurFloor()+offset);
                    addEventMessage("Ascended to floor: %d", getCurFloor()+1);
                }
                break;
            }
        }
    }

}


static void pickUpItems(){
    Map* floor = getFloor(getCurFloor());
    Player* player = getPlayerInstance();
    for (int i = 0; i < floor->num_rooms; i++){
        Room* room = floor->rooms[i];

        for (int j = 0; j < room->num_items;){
            Item* item = room->items[j];

            if (item->pos.gridX == player->pos.gridX &&
                item->pos.gridY == player->pos.gridY){
                if (!ItemOnPickup(item)){
                    addItemToPlayer(player, item);
                }
                removeItemFromRoom(room, j);
            }
            else{
                j++;
            }
        }
    }
}

static void renderPlayer(Player* player){
    renderCell(player->pos.gridX, player->pos.gridY, player->sprite, player->spriteColor, 0);
}

static void renderPlayerStatus(Player* player){
    char message[100];
    sprintf(message, "%d/%d", player->health, player->maxHealth);
    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE, COLOR_RED);
    renderString(20+calculateTextWidth(message, FONTNORMALSCALE)+3, RWINDOW_HEIGHT-53, "\u0100", FONTNORMALSCALE*0.75, COLOR_RED);

    sprintf(message, "%d", player->gold);
    renderString(270, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE, COLOR_GOLD);
    renderString(270+calculateTextWidth(message, FONTNORMALSCALE), RWINDOW_HEIGHT-50, "\u0101", FONTNORMALSCALE*0.75, COLOR_GOLD);

    sprintf(message, "%d/%d", player->armor, player->maxArmor);
    renderString(460, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE, COLOR_GRAY);
    renderString(460+calculateTextWidth(message, FONTNORMALSCALE)+3, RWINDOW_HEIGHT-50, "\u0102", FONTNORMALSCALE*0.9, COLOR_GRAY);

    sprintf(message, "%d/%d", player->hunger, player->maxHunger);
    renderString(680, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE, COLOR_BROWN);
    renderString(680+calculateTextWidth(message, FONTNORMALSCALE)+3, RWINDOW_HEIGHT-50, "\u0103", FONTNORMALSCALE*0.9, COLOR_BROWN);

    sprintf(message, "D-%d:%ds\nS-%d:%ds\nH-%d:%ds", player->multies[0], player->multiesT[0],
            player->multies[1], player->multiesT[1],
            player->multies[2], player->multiesT[2]);
    renderString(900, RWINDOW_HEIGHT-70, message, FONTNORMALSCALE, COLOR_BROWN);

    if (player->equippedItem != NULL){
        sprintf(message, "%s", player->equippedItem->name);
        renderString(1120, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE, COLOR_PURPLE);
        renderString(1120+calculateTextWidth(message, FONTNORMALSCALE)+3, RWINDOW_HEIGHT-50, player->equippedItem->sprite, FONTNORMALSCALE*0.9, COLOR_BROWN);
    }
    else{
        renderString(1120, RWINDOW_HEIGHT-50, "Empty", FONTNORMALSCALE, COLOR_PURPLE);
    }
//
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
//    renderString(20, RWINDOW_HEIGHT-50, message, FONTNORMALSCALE*3, COLOR_GOLD);
}


static void renderRoomDoors(Room* room){
    for (int i = 0; i < room->num_doors; i++){
        renderCell(room->doors[i]->pos.gridX, room->doors[i]->pos.gridY, "+", (Color) {0.6f, 0.4f, 0.2f, 1.0f}, 1);
    }
}

static void renderCorridors(Map* map){

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            if (cor->visited[j] || showMap){
                float cellWidth = gridCellWidth();
                float cellHeight = gridCellHeight();

                float xpos = (cor->path[j].gridX) * cellWidth + XBUFFER_ZONE;
                float ypos = (cor->path[j].gridY) * cellHeight + YBUFFER_ZONE;
                float w = 1 * cellWidth;
                float h = 1 * cellHeight;

                renderQuad(( Pos ){xpos, ypos}, ( Pos ){xpos+w, ypos+h}, (Color) {0.4f, 0.5f, 0.6f, 1.0f});
            }
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
        if (findEntityRoom(entity)->visited || showMap){
            renderCell(entity->pos.gridX, entity->pos.gridY, entity->sprite, entity->spriteColor, 0);

        }
    }
}

static void renderPlaytime(){
    char temp[100];
    int hours, minutes, seconds;
    formatTime(getCurrentUser()->stats.playTime, &hours, &minutes, &seconds);
    sprintf(temp, "Playtime %02d:%02d:%02d", hours, minutes, seconds);
    renderString(RWINDOW_WIDTH-calculateTextWidth(temp, FONTNORMALSCALE)-20, RWINDOW_HEIGHT-50, temp, FONTNORMALSCALE, COLOR_EMERALD);
}

static void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    Map* map = getFloor(getCurFloor());
    renderCorridors(map);

    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];
        if (room->visited || showMap){
            renderRoomBox(room->pos.gridX, room->pos.gridY, room->scale.gridW, room->scale.gridH,
                room->floorsColor, room->wallsColor);
            renderRoomDoors(room);
            renderRoomItems(room);
            renderRoomStructures(room);
        }
    }

    renderEntities(map);

    Player* player = getPlayerInstance();

    renderPlayer(player);
    renderPlayerStatus(player);
    renderEventBar();
    renderPlaytime();

    if (getCurrentSave()->gameFinished){
        // Disable inventory menu
        for (int i = 0; i < invtabs_c; i++){
            invtabs[i].enabled = 0;
        }
        showEndScreen();
    }

    if (invtabs[0].enabled){
//        glClearColor(1, 1, 1, 1);
//        glClear(GL_COLOR_BUFFER_BIT);
        updateInventoryMenu();
        renderQuad(( Pos ){50, 50}, ( Pos ){RWINDOW_WIDTH-50, RWINDOW_HEIGHT-50}, (Color) {0.5f, 0.5f, 0.5f, 0.9f} );
        renderString(1270, 100, "-Potions", FONTNORMALSCALE, COLOR_FUCHSIA);
        renderString(70, 100, "-Melee", FONTNORMALSCALE, COLOR_FUCHSIA);
        renderString(470, 100, "-Ranged", FONTNORMALSCALE, COLOR_FUCHSIA);
        renderString(870, 100, "-Foods", FONTNORMALSCALE, COLOR_FUCHSIA);
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

void calculatePlayTime(int value){
    if (strcmp(getGameInstance()->currentScene->sceneID, "game")) return;
    getCurrentUser()->stats.playTime += 1;
    glutTimerFunc(1000, calculatePlayTime, 0);
}

void onEnter(){
    revealMap();
    showMap = 0;
    glutTimerFunc(0, calculatePlayTime, 0);
}

void initscene_game(){
    endMenu.enabled = 0;
    endMenu.num_elements = 0;
    endMenu.num_interactable_elements = 0;


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

    scene->onEnter = onEnter;
    scene->onExit = NULL;
    scene->onKeypress = processKeyboard;
    scene->onSpecialKeypress = processSKeyboard;
    scene->update = render;

    // Add scene to the game
    addScene(scene);
}
