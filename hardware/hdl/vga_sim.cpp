#include "Vtop.h"
#include "raylib.h"
#include <verilated.h>

int VGA_WIDTH = 640;
int VGA_HEIGHT = 480;

int MARGIN = 50;

void step_clock(Vtop *top, size_t n) {
  for (size_t i = 0; i < n; i++) {
    top->clk = 0;
    top->eval();
    top->clk = 1;
    top->eval();
    top->clk = 0;
    top->eval();
  }
}

void step_logic_clock(Vtop *top) { step_clock(top, 1); }
void step_pixel_clock(Vtop *top) {
  top->clk25 = 0;
  top->eval();
  top->clk25 = 1;
  top->eval();
  top->clk25 = 0;
  top->eval();
}

void read_frame(Vtop *top, Color *pixels);

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);
  Vtop *top = new Vtop;

  InitWindow(VGA_WIDTH, VGA_HEIGHT + MARGIN, "VGA Viewer");
  SetTargetFPS(60);

  Color *pixels = (Color *)MemAlloc(VGA_WIDTH * VGA_HEIGHT * sizeof(Color));

  // !Verilated::gotFinish()

  top->rst = 1;
  step_clock(top, 10);
  top->rst = 0;

  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      pixels[x + y * VGA_WIDTH] = BLACK;
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
    BeginDrawing();
    ClearBackground(GRAY);

    // advance cpu
    step_logic_clock(top);

    {
      read_frame(top, pixels);
      DrawTexture(checkeredTexture, 0, MARGIN, WHITE);
      UpdateTexture(checkeredTexture, pixels);
    }

    int w = ((VGA_WIDTH - 0) - (5 * 16)) / 16;
    for (size_t i = 0; i < 16; i++) {
      if ((top->leds >> i) & 1) {
        DrawRectangle(0 + i * (w + 5), 5, w, 40, GREEN);
      } else {
        DrawRectangle(0 + i * (w + 5), 5, w, 40, RED);
      }
    }

    DrawFPS(0, 0);
    EndDrawing();
  }

  UnloadTexture(checkeredTexture);
  CloseWindow();

  delete top;
}

// read a frame from vga and update pixels array
void read_frame(Vtop *top, Color *pixels) {
  int x = 0;
  int y = 0;

  while (top->vsync) {
    step_pixel_clock(top);
  }
  // wait for vsync pulse?
  while (!top->vsync) {
    step_pixel_clock(top);
  }
  for (size_t i = 0; i < 33; i++) {
    while (top->hsync) {
      step_pixel_clock(top);
    }
    while (!top->hsync) {
      step_pixel_clock(top);
    }
  }
  while (top->vsync) {
    if (y >= VGA_HEIGHT) {
      step_pixel_clock(top);
      continue;
    }
    for (size_t i = 0; i < 48; i++) {
      step_pixel_clock(top);
    }
    while (top->hsync) {
      if (x >= VGA_WIDTH) {
        step_pixel_clock(top);
        continue;
      }

      Color pixel = {
          .r = (uint8_t)top->red,
          .g = (uint8_t)top->green,
          .b = (uint8_t)top->blue,
          .a = 255,
      };

      pixels[x + y * VGA_WIDTH] = (Color){(unsigned char)(( top->red << 4 ) | top->red << 4),
                                          (unsigned char)(top->green << 4),
                                          (unsigned char)(top->blue << 4), 255};
      // DrawRectangle(x, y + MARGIN, 1, 1, pixel);

      step_pixel_clock(top);
      x += 1;
    }

    // wait for hsync pulse?
    while (!top->hsync) {
      step_pixel_clock(top);
    }

    y += 1;
    x = 0;
  }
}
