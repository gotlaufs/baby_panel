#include <stdint.h>
#include "defines.h"
/*
 * ws2812.h
 *
 *  Created on: 21.09.2016.
 *      Author: Michael Meli
 *              Roberts Gotlaufs
 *  Original source:
 *      https://github.com/mjmeli/MSP430-NeoPixel-WS2812-Library
 *
 *  ws2812 driver library using MSP430 SPI module.
 */


// Transmit codes
#define HIGH_CODE   (0xF8)          // b11111000
//#define LOW_CODE    (0xE0)          // b11100000
#define LOW_CODE    (0x80)          // b11000000


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} WS2812_LED;

extern WS2812_LED ws2812_leds[WS2812_COUNT];

// Configure processor to output to data strip
void initStrip(void);

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip(void);

// Set the color of a certain LED
void setLEDColor(uint16_t p, uint8_t r, uint8_t g, uint8_t b);

// Set the color of a certain LED by HSV Hue angle
void setHsvColor(const uint8_t number, int16_t angle);

// Clear the color of all LEDs (make them black/off)
void clearStrip(void);

// Fill the strip with a solid color. This will update the strip.
void fillStrip(uint8_t r, uint8_t g, uint8_t b);

// Push all previous colors one LED further in strip and put new color in the firs LED.
void advanceStrip(uint8_t r, uint8_t g, uint8_t b);
