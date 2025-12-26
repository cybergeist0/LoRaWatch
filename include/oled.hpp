#pragma once

#include <stdint.h>
#include <stddef.h>
#include "hardware/i2c.h"

class OledDisplay {
public:
    // NOTE TO WHOEVER IS USING THIS
	// addr typically 0x3C for ssd1306's
    // READ YOUR DATASHEET BEFORE USING
    // NOT ALL OF THEM HAVE THE SAME ADDRESS
	OledDisplay(i2c_inst_t* i2c_port, uint sda_pin, uint scl_pin, uint8_t addr = 0x3C,
				uint8_t width = 128, uint8_t height = 64);

	bool init();
	void clear(bool color = false);
	void drawPixel(int x, int y, bool color = true);
	void drawRect(int x, int y, int w, int h, bool color = true);
	void fillRect(int x, int y, int w, int h, bool color = true);
	void drawChar(int x, int y, char c, bool color = true);
	void drawString(int x, int y, const char* str, bool color = true);
	void drawImage(int x, int y, int size, const bool* pixelMap);
	void update();  // push buffer to display

private:
	void sendCommand(uint8_t cmd);
	void sendCommands(const uint8_t* cmds, size_t count);
	void sendBufferPage(uint8_t page);

	i2c_inst_t* i2c_port;
	uint sda_pin;
	uint scl_pin;
	uint8_t addr;
	uint8_t width;
	uint8_t height;
	// 1-bit framebuffer, height is multiple of 8
	static constexpr size_t kMaxBuffer = 1024; // 128x64/8
	uint8_t buffer[kMaxBuffer];
};
