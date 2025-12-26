#include "menu.hpp"

#include <array>

static std::array<int, kNumTiles> makeDesignSizes() {
    std::array<int, kNumTiles> sizes{};
    for (int i = 0; i < kNumTiles; ++i) sizes[i] = 5 + 2 * i; // 5,7,9,...
    return sizes;
}

static const std::array<int, kNumTiles> kDesignSizes = makeDesignSizes();

static void drawTileWithSquare(OledDisplay& display, int tileIdx, int screenOffset) {
    // screenOffset: -1 for left neighbor, 0 center, +1 right neighbor
    int x = kCenterX + screenOffset * kTileSpacing;
    int y = kTileY;
    display.drawRect(x, y, kTileSize, kTileSize, true);

    int sq = kDesignSizes[tileIdx];
    if (sq > kTileSize) sq = kTileSize;
    int marginX = (kTileSize - sq) / 2;
    int marginY = (kTileSize - sq) / 2;
    display.fillRect(x + marginX, y + marginY, sq, sq, true);
}

void renderMainMenu(OledDisplay& display, int selectedIdx) {
    display.clear();
    display.drawRect(0, 0, 124, 15, true);
    display.drawString(4, 4, "4:00 10/10/2026", true); // for ds1307 in diff. feature

    int leftIdx = (selectedIdx - 1 + kNumTiles) % kNumTiles;
    int rightIdx = (selectedIdx + 1) % kNumTiles;

    drawTileWithSquare(display, leftIdx, -1);
    drawTileWithSquare(display, selectedIdx, 0);
    drawTileWithSquare(display, rightIdx, 1);

    display.update();
}
