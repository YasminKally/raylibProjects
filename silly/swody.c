#include "raylib.h"
#include <stdio.h>
#include <math.h>

int moveX(float amount, Rectangle* player, Rectangle surfaces[]);
void moveY(float* amount, Rectangle* player, Rectangle surfaces[], int* jumped, int* onLand);
int collision(Rectangle r, Rectangle surfaces[]);

int main(void) {
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const int horizontalSpeed = 12;
    const int jumpStrength = 17;
    int jumped = 0;
    int onLand = 0;
    int slowTime = 0;
    int coyoteTime = 0;
    Vector2 velocity = {0, 0};

    InitWindow(screenWidth, screenHeight, "Collision");
    ToggleFullscreen();
    SetTargetFPS(60);


    Rectangle floor = {0, screenHeight / 2 + 450, screenWidth, screenHeight / 2};
    Rectangle wall = {screenWidth / 2 - 300, floor.y - 100, 600, 100};
    Rectangle rightWall = {screenWidth - 15, 0, 15, screenHeight};
    Rectangle leftWall = {0, 0, 15, screenHeight};
    Rectangle platform = {screenWidth / 2 - 150, wall.y - 150, 300, 40};

    Rectangle surfaces[] = {floor, wall, rightWall, leftWall, platform};

    Rectangle player = {50, floor.y - 40, 40, 40};

    while (!WindowShouldClose())
    {
        onLand = 1;

        // player movement
        if(slowTime > 0){
            slowTime--;
            velocity.x += horizontalSpeed * 0.1f * (IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
            velocity.x -= horizontalSpeed * 0.1f * (IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
        } else {
            velocity.x += horizontalSpeed * 0.8f * (IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
            velocity.x -= horizontalSpeed * 0.8f * (IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
        };
        int direction = (velocity.x > 0) - (velocity.x < 0);
        if(abs(velocity.x) > horizontalSpeed) {
            velocity.x = horizontalSpeed * direction;
        };
        if(floorf(velocity.x) == 0) {
            velocity.x = 0;
        };
        if(!(IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) && !(IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))) {
            if (velocity.x != 0)
            {
                velocity.x -= direction;
            };
        };
        int onWall = moveX(velocity.x, &player, surfaces);

        // floor jump
        if((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) && !jumped && (onLand || coyoteTime > 0)) {
            if(coyoteTime > 0) {
                velocity.y = 0;
                coyoteTime = 0;
            };
            velocity.y -= jumpStrength;
            jumped = 1;
            onLand = 0;
        };
        if(coyoteTime > 0) {
            coyoteTime--;
        };
        if(!onLand) {
            coyoteTime = 2;
        };
        moveY(&velocity.y, &player, surfaces, &jumped, &onLand);

        // wall jump
        if((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) && onWall && !onLand && velocity.y > -(jumpStrength / 2)){
            slowTime = 20;
            velocity.x -= direction * horizontalSpeed * 88;
            velocity.y -= jumpStrength * 1.3f;
        };

        // gravity
        if(velocity.y != 0 || !onLand) {
            velocity.y += 1.3f;
        };

        BeginDrawing(); 
                ClearBackground(BLACK);
                DrawRectangleRec(floor, DARKGRAY);
                DrawRectangleRec(wall, DARKGRAY);
                DrawRectangleRec(rightWall, DARKGRAY);
                DrawRectangleRec(leftWall, DARKGRAY);
                DrawRectangleRec(platform, DARKGRAY);

                DrawRectangleRec(player, YELLOW);
        EndDrawing();
    };

    CloseWindow();
    return 0;
};

int moveX(float amount, Rectangle* player, Rectangle surfaces[]) {
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
                return true;
            };
        };
    };
    return false;
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
    for(int i = 0; i < 5; i += 1){
        if(CheckCollisionRecs(r, surfaces[i])) {
            return true;
        };
    };
    return false;
};