#include "swody.h"
#include <stdio.h>
#include <math.h>

const int screenWidth = 1920;
const int screenHeight = 1080;
const int horizontalSpeed = 12;
const int jumpStrength = 22;

int main(void) {

    InitWindow(screenWidth, screenHeight, "Swody");
    ToggleFullscreen();
    SetTargetFPS(60);

    Rectangle floor = {0, screenHeight / 2 + 450, screenWidth, screenHeight / 2};
    Rectangle wall = {screenWidth / 2 - 300, floor.y - 100, 600, 100};
    Rectangle rightWall = {screenWidth - 15, 0, 15, screenHeight};
    Rectangle leftWall = {0, 0, 15, screenHeight};
    Rectangle platform = {screenWidth / 2 - 150, wall.y - 150, 300, 40};

    Rectangle surfaces[] = {floor, wall, rightWall, leftWall, platform};

    Player player1 = {0};
    Player player2 = {0};
    reset(&player1, &player2);
    Input input1 = {KEY_A, KEY_D, KEY_SPACE, KEY_S, 0};
    Input input2 = {KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, 1};

    while (!WindowShouldClose()) {
        if(IsKeyPressed(KEY_R)){
            reset(&player1, &player2);
        };
        movePlayer(&player1, input1, surfaces);
        movePlayer(&player2, input2, surfaces);
        playerCollision(&player1, &player2, surfaces);
        playerCollision(&player2, &player1, surfaces);

        BeginDrawing(); 
            ClearBackground(BLACK);
            DrawRectangleRec(floor, DARKGRAY);
            DrawRectangleRec(wall, DARKGRAY);
            DrawRectangleRec(rightWall, DARKGRAY);
            DrawRectangleRec(leftWall, DARKGRAY);
            DrawRectangleRec(platform, DARKGRAY);

            DrawRectangleRec(player1.body, YELLOW);
            DrawRectangleRec(player2.body, PINK);
        EndDrawing();
    };

    CloseWindow();
    return 0;
};

int reset(Player* p1, Player* p2){
    p1->body = (Rectangle){50, screenHeight / 2 + 410, 40, 40};
    p1->velocity = (Vector2){0};
    p2->body = (Rectangle){screenWidth - 50 - 40, screenHeight / 2 + 410, 40, 40};
    p2->velocity = (Vector2){0};
};

void movePlayer(Player* player, Input input, Rectangle surfaces[]){
    player->onLand = 1;

    // player movement
    if(player->slowTime > 0){
        player->slowTime--;
        player->velocity.x += horizontalSpeed * 0.1f * (IsKeyDown(input.right) ||
            IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
        player->velocity.x -= horizontalSpeed * 0.1f * (IsKeyDown(input.left) ||
            IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
    } else {
        player->velocity.x += horizontalSpeed * 0.8f * (IsKeyDown(input.right) ||
            IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
        player->velocity.x -= horizontalSpeed * 0.8f * (IsKeyDown(input.left) ||
            IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
    };
    int direction = (player->velocity.x > 0) - (player->velocity.x < 0);
    if(abs(player->velocity.x) > horizontalSpeed) {
        player->velocity.x = horizontalSpeed * direction;
    };
    if(floorf(player->velocity.x) == 0) {
        player->velocity.x = 0;
    };
    if(!(IsKeyDown(input.right) || IsKeyDown(input.left)) &&
        !(IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
        IsGamepadButtonDown(input.gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT))){
        if (player->velocity.x != 0)
        {
            player->velocity.x -= direction;
        };
    };
    int onWall = moveX(player->velocity.x, &player->body, surfaces);

    // floor jump
    if((IsKeyPressed(input.up) || IsGamepadButtonPressed(input.gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) &&
        !player->jumped && (player->onLand || player->coyoteTime > 0)){
        if(player->coyoteTime > 0) {
            player->velocity.y = 0;
            player->coyoteTime = 0;
        };
        player->velocity.y -= jumpStrength;
        player->jumped = 1;
        player->onLand = 0;
    };
    if(player->coyoteTime > 0){
        player->coyoteTime--;
    };
    if(!player->onLand){
        player->coyoteTime = 2;
    };
    moveY(&player->velocity.y, &player->body, surfaces, &player->jumped, &player->onLand);

    // wall jump
    if((IsKeyPressed(input.up) || IsGamepadButtonPressed(input.gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) &&
        onWall && !player->onLand && player->velocity.y > -(jumpStrength / 2)){
        player->slowTime = 20;
        player->velocity.x -= direction * horizontalSpeed * 88;
        player->velocity.y -= jumpStrength * 1.3f;
    };

    // gravity
    if(player->velocity.y != 0 || !player->onLand){
        player->velocity.y += 1.3f;
    };
};

int moveX(float amount, Rectangle* player, Rectangle surfaces[]){
    static float remainder = 0;
    remainder += amount;
    int move = (int)remainder;

    if(move) {
        remainder -= move;
        int sign = (move > 0) - (move < 0);

        while(move != 0) {
            player->x += sign;
            if(!collision(*player, surfaces)){
                move -= sign;
            } 
            else {
                player->x -= sign;
                return true;
            };
        };
    };
    return false;
};

void moveY(float* amount, Rectangle* player, Rectangle surfaces[], int* jumped, int* onLand){
    static float remainder = 0;
    remainder += *amount;
    int move = (int)remainder;
    *onLand = 0;

    if(move) {
        remainder -= move;
        int sign = (move > 0) - (move < 0);

        while(move != 0) {
            player->y += sign;
            if(!collision(*player, surfaces)){
                move -= sign;
            } 
            else {
                player->y -= sign;
                *amount = 0;
                *jumped = 0;
                *onLand = 1;
                break;
            };
        };
    };
};

int collision(Rectangle r, Rectangle surfaces[]){ 
    for(int i = 0; i < 5; i += 1){
        if(CheckCollisionRecs(r, surfaces[i])){
            return true;
        };
    };
    return false;
};

int playerCollision(Player* ref, Player* other, Rectangle surfaces[]){
    if(CheckCollisionRecs(ref->body, other->body)){
        // overhead collision
        if(ref->body.y + ref->body.height < other->body.y + other->body.height){
            int delta = 0;
            if(ref->body.x < other->body.x){
                delta = (other->body.x - ref->body.width) - ref->body.x;
            } else {
                delta = (other->body.x + other->body.width) - ref->body.x;
            };
            // TODO: edge case when falling onto a wall
            ref->velocity.x += delta;
        } else {
            int delta = 0;
            if(ref->body.x < other->body.x){
                delta = (ref->body.x + ref->body.width) - other->body.x;
            } else {
                delta = ref->body.x - (other->body.x + other->body.width);
            };
            if(ref->velocity.x != 0 && other->velocity.x == 0){
                int col = moveX(delta, &other->body, surfaces);
                if(col){
                    ref->velocity.x = 0;
                    if(ref->body.x < other->body.x){
                        ref->body.x = other->body.x - ref->body.width;
                    } else {
                        ref->body.x = other->body.x + other->body.width;
                    };
                };
            } else if(ref->velocity.x != 0){
                ref->velocity.x = 0;
                other->velocity.x = 0;
                ref->body.x -= delta / 2;
                other->body.x += delta / 2;
            };
        };
    };
};