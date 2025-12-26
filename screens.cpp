#include "screens.hpp"

#include <array>
#include <stdio.h>

static void renderScreenTemplate(OledDisplay& display, char id, int tileIdx) {
    display.clear();
    display.drawRect(0, 0, 124, 15, true);

    char line[24];
    snprintf(line, sizeof(line), "Mode %c", id);
    display.drawString(4, 4, line, true);

    snprintf(line, sizeof(line), "Tile %d", tileIdx + 1);
    display.drawString(4, 20, line, true);

    // filler
    int size = 10 + (tileIdx * 2);
    if (size > 40) size = 40;
    int x = (128 - size) / 2;
    int y = 40;
    display.drawRect(x, y, size, size, true);

    display.update();
}

static std::array<Screen, kNumTiles> makeScreens() {
    std::array<Screen, kNumTiles> screens{};
    for (int i = 0; i < kNumTiles; ++i) {
        char id = static_cast<char>('A' + i);
        screens[i] = Screen{ id, [i, id](OledDisplay& d) {
            renderScreenTemplate(d, id, i);
        } };
    }
    return screens;
}

const Screen& screenForTile(int tileIndex) {
    static const std::array<Screen, kNumTiles> screens = makeScreens();
    int idx = ((tileIndex % kNumTiles) + kNumTiles) % kNumTiles;
    return screens[idx];
}
