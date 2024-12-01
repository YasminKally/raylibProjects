#include "raylib.h"


int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int horizontalSpeed = 12;
    float verticalAcceleration = 0;
    const int jumpStrengh = 30;
    int jumped = 0;
    int floor = screenHeight / 2 + 150;

    InitWindow(screenWidth, screenHeight, "Player");
    SetTargetFPS(60);

    Rectangle player = {screenWidth / 2 - 20, floor - 20, 40, 40};

    while(!WindowShouldClose() && !IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT)) {
        //player movement
        player.x -= horizontalSpeed * (IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT));
        player.x += horizontalSpeed * (IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT));
        
        //crouch
        int crouched = IsKeyDown(KEY_S) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        player.height = 40 / (1 + crouched);

        //player jump
        if ((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) && !jumped) {
            jumped = 1;
            verticalAcceleration -= jumpStrengh;
        };
        player.y += verticalAcceleration;
        if (verticalAcceleration != 0) { //gravity
            verticalAcceleration += 2.7f;
        };
        if(player.y + player.height >= floor) { //landing
            verticalAcceleration = 0;
            player.y = floor - (player.height / 2);
            jumped = 0;
        };

        //global collision
        while (player.x + 40 > screenWidth) {
            player.x--;
        };
        while (player.x < 0) {
            player.x++;
        };

        BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangle(0, floor + 20, screenWidth, floor, DARKGRAY);

            DrawRectangle(player.x, player.y + 10 * crouched, player.width, player.height, PINK);
        EndDrawing();
    };

    CloseWindow();
    return 0;
};
