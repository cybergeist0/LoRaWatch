#pragma once
#include <stdint.h>
#include <functional>
#include "pico/stdlib.h"

class Button {
public:
    Button(uint pin);
    void init();
    void setOnPress(std::function<void()> callback);
    void setOnRelease(std::function<void()> callback);
    void setDebounceMs(uint32_t ms);
    bool isPressed();
private:
    uint pin;
    uint32_t debounce_ms;
    uint32_t last_irq_ms;
    bool last_state;
    bool pressed;
    std::function<void()> on_press_callback;
    std::function<void()> on_release_callback;
    static void gpio_callback(uint gpio, uint32_t events);
    static Button* instances[30];
};