#include "rotary.hpp"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "gpio_manager.hpp"

RotaryEncoder::RotaryEncoder(uint clk_pin, uint dt_pin)
    : clk_pin(clk_pin), dt_pin(dt_pin), count(0), previous_clk(false), last_irq_ms(0), debounce_ms(3) {
}

void RotaryEncoder::init() {
    gpio_init(clk_pin);
    gpio_set_dir(clk_pin, GPIO_IN);
    gpio_pull_up(clk_pin);
    gpio_init(dt_pin);
    gpio_set_dir(dt_pin, GPIO_IN);
    gpio_pull_up(dt_pin);
    previous_clk = gpio_get(clk_pin);
    last_irq_ms = to_ms_since_boot(get_absolute_time());
    RotaryEncoder::instance = this;
    // both to track state but only count on falling
    gpio_manager_register_rotary(clk_pin, &RotaryEncoder::gpio_callback);
}

void RotaryEncoder::gpio_callback(uint gpio, uint32_t events) {
    if (RotaryEncoder::instance == nullptr) return;
    RotaryEncoder* enc = RotaryEncoder::instance;
    if (gpio != enc->clk_pin) return;

    uint32_t now_ms = to_ms_since_boot(get_absolute_time());
    if (now_ms - enc->last_irq_ms < enc->debounce_ms) return;
    enc->last_irq_ms = now_ms;

    bool clk_state = gpio_get(enc->clk_pin);
    bool dt_state = gpio_get(enc->dt_pin);

    if (enc->previous_clk && !clk_state) {
        if (dt_state) {
            enc->count--;
        } else {
            enc->count++;
        }
    }

    enc->previous_clk = clk_state;
}

int RotaryEncoder::getCount() {
    return count;
}

void RotaryEncoder::resetCount() {
    count = 0;
}

void RotaryEncoder::setDebounceMs(uint32_t ms) {
    debounce_ms = ms;
}