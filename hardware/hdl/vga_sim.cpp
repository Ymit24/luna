#include "Vvga.h"
#include "raylib.h"
#include <vector>
#include <verilated.h>

int VGA_WIDTH = 640;
int VGA_HEIGHT = 640;

int MARGIN = 50;

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  Vvga *top = new Vvga;

  InitWindow(VGA_WIDTH + MARGIN, VGA_HEIGHT + MARGIN, "VGA Viewer");
  SetTargetFPS(60);

  Color *pixels = (Color *)MemAlloc(VGA_WIDTH * VGA_HEIGHT * sizeof(Color));

  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      if ((x / 32 + y / 32) % 2 == 0) {
        pixels[x + y * VGA_WIDTH] = RED;
      } else {
        pixels[x + y * VGA_WIDTH] = GREEN;
      }
    }
  }

  Image checkeredImage = {
      .data = pixels,
      .width = VGA_WIDTH,
      .height = VGA_HEIGHT,
      .mipmaps = 1,
      .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Texture2D checkeredTexture = LoadTextureFromImage(checkeredImage);
  UnloadImage(checkeredImage);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(checkeredTexture, MARGIN / 2, MARGIN / 2, WHITE);
    EndDrawing();
  }

  CloseWindow();

  delete top;
}
