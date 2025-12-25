#include <stdio.h>
#include "pico/stdlib.h"
#include "rotary.hpp"

const uint CLK_PIN = 0;
const uint DT_PIN = 1;

int main() {
    stdio_init_all();
    RotaryEncoder enc(CLK_PIN, DT_PIN);
    enc.setDebounceMs(1);
    enc.init();
    while (true) {
        int val = enc.getCount();
        printf("Count: %d\n", val);
        sleep_ms(100);
    }
}