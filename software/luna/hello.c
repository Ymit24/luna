#include "raylib.h"

int main(void) {
    // Initialize the window
    InitWindow(800, 450, "Hello Raylib");
    
    // Set target FPS for smoother visuals
    SetTargetFPS(60);
    
    // Main loop
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, world from Raylib!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    // Clean up
    CloseWindow();
    return 0;
}
