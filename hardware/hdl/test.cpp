#include "raylib.h"
#include <verilated.h>
#include "Vvga.h"
#include <vector>

#define VISIBLE_WIDTH 640
#define VISIBLE_HEIGHT 480

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Vvga* top = new Vvga;

    // Raylib Initialization
    InitWindow(VISIBLE_WIDTH, VISIBLE_HEIGHT, "Verilator + Raylib VGA Monitor");
    SetTargetFPS(60);

    // Create an Image and Texture to store the VGA frame
    // We use a CPU-side Image to manipulate pixels, then push to GPU Texture
    Image screenImage = {
        .data = malloc(VISIBLE_WIDTH * VISIBLE_HEIGHT * 4),
        .width = VISIBLE_WIDTH,
        .height = VISIBLE_HEIGHT,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };
    Texture2D screenTexture = LoadTextureFromImage(screenImage);
    Color* pixels = (Color*)screenImage.data;

    int h_pos = 0;
    int v_pos = 0;
    bool prev_hsync = 1;
    bool prev_vsync = 1;

    top->rst = 1;
    for (int i = 0; i < 10; i++) { top->clk = 0; top->eval(); top->clk = 1; top->eval(); }
    top->rst = 0;

    while (!WindowShouldClose() && !Verilated::gotFinish()) {
        // Run until we hit a VSync (the end of a frame)
        bool frame_ready = false;
        
        while (!frame_ready) {
            top->clk = 0; top->eval();
            top->clk = 1; top->eval();
            
            top->bram_doutb = top->bram_addrb; // BRAM Emulation

            // HSync Falling Edge
            if (prev_hsync && !top->hsync) {
                h_pos = 0;
                v_pos++;
            }
            // VSync Falling Edge
            if (prev_vsync && !top->vsync) {
                v_pos = 0;
                frame_ready = true; // One frame is fully rendered in the buffer
            }

            // Capture pixels
            if (h_pos < VISIBLE_WIDTH && v_pos < VISIBLE_HEIGHT) {
                pixels[v_pos * VISIBLE_WIDTH + h_pos] = (Color){
                    (unsigned char)(top->red << 4),
                    (unsigned char)(top->green << 4),
                    (unsigned char)(top->blue << 4),
                    255
                };
            }

            h_pos++;
            prev_hsync = top->hsync;
            prev_vsync = top->vsync;
        }

        // Display the frame
        UpdateTexture(screenTexture, pixels);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(screenTexture, 0, 0, WHITE);
            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(screenTexture);
    free(screenImage.data);
    CloseWindow();
    delete top;
    return 0;
}
