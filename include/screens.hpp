#pragma once

#include <functional>

#include "oled.hpp"
#include "app_config.hpp"

struct Screen {
    char id;
    std::function<void(OledDisplay&)> render;
};

const Screen& screenForTile(int tileIndex);
