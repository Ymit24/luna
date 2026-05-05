#include "Vtop.h"
#include "raylib.h"
#include <verilated.h>

int VGA_WIDTH = 640;
int VGA_HEIGHT = 480;

int MARGIN = 50;

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  Vtop *top = new Vtop;

  InitWindow(VGA_WIDTH, VGA_HEIGHT + MARGIN, "VGA Viewer");
  SetTargetFPS(60);

  Color *pixels = (Color *)MemAlloc(VGA_WIDTH * VGA_HEIGHT * sizeof(Color));

  // !Verilated::gotFinish()

  top->rst = 1;
  for (int i = 0; i < 10; i++) {
    top->raw_clk = 0;
    top->eval();
    top->raw_clk = 1;
    top->eval();
  }
  top->rst = 0;

  for (int i = 0; i < 100; i++) {
    top->raw_clk = 0;
    top->eval();
    top->raw_clk = 1;
    top->eval();
  }

  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      pixels[x + y * VGA_WIDTH] = (Color){(unsigned char)(top->red << 4),
                                          (unsigned char)(top->green << 4),
                                          (unsigned char)(top->blue << 4), 255};
      // } else {
      //   pixels[x + y * VGA_WIDTH] = GREEN;
      // }
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
  // UnloadImage(checkeredImage);

  while (!WindowShouldClose()) {
    top->raw_clk = 0;
    top->eval();
    top->raw_clk = 1;
    top->eval();

    BeginDrawing();
    ClearBackground(GRAY);
    DrawTexture(checkeredTexture, 0, MARGIN, WHITE);
    UpdateTexture(checkeredTexture, pixels);

    int w = ((VGA_WIDTH - 0) - (5 * 16)) / 16;
    for (size_t i = 0; i < 16; i++) {
      if ((top->top__02Eleds >> i) & 1) {
        DrawRectangle(0 + i * (w + 5), 5, w, 40, WHITE);
      }
    }

    EndDrawing();
  }

  UnloadTexture(checkeredTexture);
  CloseWindow();

  delete top;
}
