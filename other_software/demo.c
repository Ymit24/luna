void start() {
    VRAM* vram = vram_init();
    if (vram_draw(vram, 10, 10, (Pixel) {
            .red = 255,
            .green = 255,
            .blue = 255,
        }) != 0) {
        panic();
    }

    if (vram_box(vram, 2, 2, 6, 6, (Pixel) {
            .red = 5,
            .green = 10,
            .blue = 255,
        }) != 0) {
        panic();
    }
}

word factorial(word n) {
    if (n <= 1) { return 1; }
    return n * factorial (n-1);
}
