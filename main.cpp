#include <stdio.h>
#include "pico/stdlib.h"
#include "rotary.hpp"
#include "button.hpp"
#include "oled.hpp"
#include "app_config.hpp"
#include "menu.hpp"
#include "screens.hpp"

const uint CLK_PIN = 0;
const uint DT_PIN = 1;
const uint BLUE_BUTTON_PIN = 16;
const uint RED_BUTTON_PIN = 17;
const uint ROTARY_BUTTON_PIN = 2;

void rotaryTest() {
    RotaryEncoder enc(CLK_PIN, DT_PIN);
    enc.setDebounceMs(1);
    enc.init();
    while (true) {
        int val = enc.getCount();
        printf("Count: %d\n", val);
        sleep_ms(100);
    }
}

void buttonTest() {
    Button blueBtn(BLUE_BUTTON_PIN), redBtn(RED_BUTTON_PIN), rotaryBtn(ROTARY_BUTTON_PIN);

    blueBtn.setDebounceMs(20); 
    blueBtn.init();
    blueBtn.setOnPress([]() {
        printf("BLUE pressed\n");
    });
    blueBtn.setOnRelease([]() {
        printf("BLUE released\n");
    });
    printf("BLUE init.\n");

    redBtn.setDebounceMs(20);
    redBtn.init();
    redBtn.setOnPress([]() {
        printf("RED pressed\n");
    });
    redBtn.setOnRelease([]() {
        printf("RED released\n");
    });
    printf("RED init.\n");

    rotaryBtn.setDebounceMs(20);
    rotaryBtn.init();
    rotaryBtn.setOnPress([]() {
        printf("ROTARY pressed\n");
    });
    rotaryBtn.setOnRelease([]() {
        printf("ROTARY released\n");
    });
    printf("ROTARY init.\n");

    
    while (true) {
        tight_loop_contents();
    }
}

void oledDemo() {
    OledDisplay display(i2c0, 4, 5);
    display.init();

    RotaryEncoder enc(CLK_PIN, DT_PIN);
    enc.setDebounceMs(1);
    enc.init();

    Button blueBtn(BLUE_BUTTON_PIN), redBtn(RED_BUTTON_PIN);
    blueBtn.setDebounceMs(20);
    redBtn.setDebounceMs(20);
    blueBtn.init();
    redBtn.init();

    volatile bool bluePressed = false;
    volatile bool redPressed = false;

    blueBtn.setOnPress([&]() { bluePressed = true; });
    redBtn.setOnPress([&]() { redPressed = true; });

    int lastSelection = -1;
    const Screen* activeScreen = nullptr;

    auto currentSelection = [&]() {
        int count = enc.getCount();
        return ((count % kNumTiles) + kNumTiles) % kNumTiles;
    };

    // initial render
    renderMainMenu(display, 0);
    lastSelection = 0;

    while (true) {
        if (!activeScreen) {
            int selected = currentSelection();
            if (selected != lastSelection) {
                renderMainMenu(display, selected);
                lastSelection = selected;
            }
            if (bluePressed) {
                bluePressed = false;
                activeScreen = &screenForTile(selected);
                if (activeScreen->render) activeScreen->render(display);
            }
        } else {
            if (redPressed) {
                redPressed = false;
                activeScreen = nullptr;
                lastSelection = -1; // force refresh of main on return
            }
            if (bluePressed) bluePressed = false;
        }
        // printf("count: %d\n", enc.getCount());
        sleep_ms(30);
    }
}

int main() {
    stdio_init_all();
    oledDemo();
    return 0;
}