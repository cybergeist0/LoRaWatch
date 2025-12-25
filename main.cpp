#include <stdio.h>
#include "pico/stdlib.h"
#include "rotary.hpp"
#include "button.hpp"

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

int main() {
    stdio_init_all();
    return 0;
}