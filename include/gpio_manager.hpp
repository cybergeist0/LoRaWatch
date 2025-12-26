#pragma once

#include "hardware/gpio.h"

// Unified GPIO interrupt manager
// Call this once to set up the shared callback
void gpio_manager_init();

// Register handlers for specific pins
void gpio_manager_register_rotary(uint clk_pin, void (*handler)(uint, uint32_t));
void gpio_manager_register_button(uint pin, void (*handler)(uint, uint32_t));
