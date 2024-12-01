#include "raylib.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int horizontalSpeed = 4;
    float verticalAcceleration = 0;
    const int jumpStrengh = 30;
    int jumped = 0;

    InitWindow(screenWidth, screenHeight, "Player");
    SetTargetFPS(60);

    Rectangle player = {screenWidth / 2 - 20, screenHeight / 2 - 20, 40, 40};

    while(!WindowShouldClose()) {
        player.x -= horizontalSpeed * IsKeyDown(KEY_A);
        player.x += horizontalSpeed * IsKeyDown(KEY_D);

        //player lateral movement
        while (player.x + 40 > screenWidth) {
            player.x--;
        };
        while (player.x < 0) {
            player.x++;
        };

        //player jump
        if (IsKeyPressed(KEY_SPACE) && !jumped) {
            jumped = 1;
            verticalAcceleration -= jumpStrengh;
        };
        player.y += verticalAcceleration;
        if (verticalAcceleration != 0) { //gravity
            verticalAcceleration += 2.7f;
        };
        if(player.y + 40 >= screenHeight / 2) { //landing
            verticalAcceleration = 0;
            player.y = screenHeight / 2 - 20;
            jumped = 0;
        };

        BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangle(0, screenHeight / 2 + 20, screenWidth, screenHeight / 2, DARKGRAY);

            DrawRectangleRec(player, PINK);
        EndDrawing();
    };

    CloseWindow();
    return 0;
};
