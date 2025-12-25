#include "button.hpp"
#include "hardware/gpio.h"
#include "pico/time.h"

Button* Button::instances[30] = { nullptr };

Button::Button(uint pin)
    : pin(pin), debounce_ms(20), last_irq_ms(0), last_state(true), pressed(false) {
}

void Button::init() {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    last_state = gpio_get(pin);
    last_irq_ms = to_ms_since_boot(get_absolute_time());
    if (pin < 30) {
        Button::instances[pin] = this;
    }
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &Button::gpio_callback);
}

void Button::gpio_callback(uint gpio, uint32_t events) {
    if (gpio >= 30 || Button::instances[gpio] == nullptr) return;
    
    Button* btn = Button::instances[gpio];
    
    uint32_t now_ms = to_ms_since_boot(get_absolute_time());
    if (now_ms - btn->last_irq_ms < btn->debounce_ms) return;
    btn->last_irq_ms = now_ms;
    
    bool current_state = gpio_get(btn->pin);
    
    // hi -> lo
    if (btn->last_state && !current_state && !btn->pressed) {
        btn->pressed = true;
        if (btn->on_press_callback) {
            btn->on_press_callback();
        }
    }

    // lo -> hi
    else if (!btn->last_state && current_state && btn->pressed) {
        btn->pressed = false;
        if (btn->on_release_callback) {
            btn->on_release_callback();
        }
    }
    
    btn->last_state = current_state;
}

void Button::setOnPress(std::function<void()> callback) {
    on_press_callback = callback;
}

void Button::setOnRelease(std::function<void()> callback) {
    on_release_callback = callback;
}

void Button::setDebounceMs(uint32_t ms) {
    debounce_ms = ms;
}

bool Button::isPressed() {
    return pressed;
}