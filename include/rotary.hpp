#pragma once
#include <stdio.h>
#include "pico/stdlib.h"

class RotaryEncoder {
public:
    RotaryEncoder(uint clk_pin, uint dt_pin);
    void init();
    int getCount();
    void resetCount();
    void setDebounceMs(uint32_t ms);
private:
    uint clk_pin;
    uint dt_pin;
    volatile int count;
    bool previous_clk;
    uint32_t last_irq_ms;
    uint32_t debounce_ms;
    static void gpio_callback(uint gpio, uint32_t events);
    inline static RotaryEncoder* instance = nullptr;
};