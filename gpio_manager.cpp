#include "gpio_manager.hpp"
#include <array>

struct GpioHandler {
    void (*callback)(uint, uint32_t);
};

static std::array<GpioHandler, 30> handlers{};
static bool initialized = false;

static void shared_gpio_callback(uint gpio, uint32_t events) {
    if (gpio < 30 && handlers[gpio].callback) {
        handlers[gpio].callback(gpio, events);
    }
}

void gpio_manager_init() {
    if (!initialized) {
        for (auto& h : handlers) {
            h.callback = nullptr;
        }
        initialized = true;
    }
}

void gpio_manager_register_rotary(uint clk_pin, void (*handler)(uint, uint32_t)) {
    if (clk_pin < 30) {
        handlers[clk_pin].callback = handler;
        if (!initialized) {
            gpio_set_irq_enabled_with_callback(clk_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, shared_gpio_callback);
            initialized = true;
        } else {
            gpio_set_irq_enabled(clk_pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
        }
    }
}

void gpio_manager_register_button(uint pin, void (*handler)(uint, uint32_t)) {
    if (pin < 30) {
        handlers[pin].callback = handler;
        if (!initialized) {
            gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, shared_gpio_callback);
            initialized = true;
        } else {
            gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);
        }
    }
}
