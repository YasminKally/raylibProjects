#include "raylib.h"
#include <stdio.h>

void moveX(float amount, Rectangle* player, Rectangle surfaces[]);
void moveY(float* amount, Rectangle* player, Rectangle surfaces[], int* jumped, int* onLand);
int collision(Rectangle r, Rectangle surfaces[]);

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int horizontalSpeed = 12;
    const int jumpStrength = 17;
    int jumped = 0;
    int onLand = 0;
    float verticalAcceleration = 1.7f;

    InitWindow(screenWidth, screenHeight, "Collision");
    SetTargetFPS(60);

    Rectangle floor = {0, screenHeight / 2 + 150, screenWidth, screenHeight - (screenHeight / 2 + 150)};
    Rectangle wall = {screenWidth / 2 - 100, floor.y - 100, 200, 100};
    Rectangle platform = {screenWidth / 2 - 40, wall.y - 100, 80, 20};

    Rectangle surfaces[] = {floor, wall, platform};

    Rectangle player = {0, floor.y - 40, 40, 40};

    while (!WindowShouldClose())
    {
        onLand = 1;

        // player movement
        int move = 0;
        move += horizontalSpeed * IsKeyDown(KEY_D);
        move -= horizontalSpeed * IsKeyDown(KEY_A);
        moveX(move, &player, surfaces);

        // player jump
        if(IsKeyPressed(KEY_SPACE) && !jumped) {
            verticalAcceleration -= jumpStrength;
            jumped = 1;
            onLand = 0;
        };
        moveY(&verticalAcceleration, &player, surfaces, &jumped, &onLand);

        // gravity
        if(verticalAcceleration != 0 || !onLand) {
            verticalAcceleration += 1.3f;
        };

        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangleRec(floor, DARKGRAY);
            DrawRectangleRec(wall, DARKGRAY);
            DrawRectangleRec(platform, DARKGRAY);

            DrawRectangleRec(player, YELLOW);
        EndDrawing();
    };

    CloseWindow();
    return 0;
};

void moveX(float amount, Rectangle* player, Rectangle surfaces[]) {
    static float remainder = 0;
    remainder += amount;
    int move = (int)remainder;

    if(move) {
        remainder -= move;
        int sign = (move > 0) - (move < 0);

        while(move != 0) {
            player->x += sign;
            if(!collision(*player, surfaces)) {
                move -= sign;
            } 
            else {
                player->x -= sign;
                break;
            };
        };
    };
};

void moveY(float* amount, Rectangle* player, Rectangle surfaces[], int* jumped, int* onLand) {
    static float remainder = 0;
    remainder += *amount;
    int move = (int)remainder;
    *onLand = 0;

    if(move) {
        remainder -= move;
        int sign = (move > 0) - (move < 0);

        while(move != 0) {
            player->y += sign;
            if(!collision(*player, surfaces)) {
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
    for(int i = 0; i < 3; i += 1){
        if(CheckCollisionRecs(r, surfaces[i])) {
            return true;
        };
    };
    return false;
};