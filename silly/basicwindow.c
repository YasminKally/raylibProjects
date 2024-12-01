#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int screenWidth = 800;
const int screenHeight = 450;

int main(void) {
    InitWindow(screenWidth, screenHeight, "basic window");

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("MORNING SAILOR!", 315, 220, 20, RAYWHITE);
        EndDrawing();
    };
    CloseWindow();
    return 0;
};