#include <stdint.h>
#include <msp430.h>
#include "ws2812.h"
#include "defines.h"
#include "half-sine.h"

/*
 * ws2812.c
 *
 *  Created on: 21.09.2016.
 *      Author: Michael Meli
 *              Roberts Gotlaufs
 *  Original source:
 *      https://github.com/mjmeli/MSP430-NeoPixel-WS2812-Library
 *
 *  ws2812 driver library using MSP430 SPI module.
 */


#define OUTPUT_PIN  (BIT0)          // Set to whatever UCB0SIMO is on your processor (Px.7 here)


WS2812_LED ws2812_leds[WS2812_COUNT] = {{0,0,0}};

// Initializes everything needed to use this library. This clears the strip.
void initStrip_old(){
    P3SEL |= OUTPUT_PIN;            // configure output pin as SPI output
    // P1SEL2 |= OUTPUT_PIN;
    UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;    // 3-pin, MSB, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;           // SMCLK source (16 MHz)
    UCB0BR0 = 6;                    // 25 MHz / 3 = .12 us per bit, 3bits = 0.36us '0' bit pulse
    								// Datasheet says 35us +- 0.15us
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;           // Initialize USCI state machine

    clearStrip();                   // clear the strip
}

void initStrip(){
    P3DIR |= OUTPUT_PIN;
    P3OUT &= ~OUTPUT_PIN;
    P3DS |= OUTPUT_PIN;

    clearStrip();                   // clear the strip
}

// Sets the color of a certain LED (0 indexed)
void setLEDColor(uint16_t p, uint8_t r, uint8_t g, uint8_t b){
//  ws2812_leds[p].red = r;
//  ws2812_leds[p].green = g;
//  ws2812_leds[p].blue = b;
// Chinese LED's have different sequence
    ws2812_leds[p].red = g;
    ws2812_leds[p].green = r;
    ws2812_leds[p].blue = b;
}

// Set the color of a certain LED by HSV Hue angle (0 indexed)
void setHsvColor(const uint8_t number, int16_t angle){
    /*
     * Wrapper for setLEDColor to use HSV angle value.
     *
     * Accepts angle in full signed int range and will
     * auto wrap to [0..360] range before calculations
     */

    // Check negative angle
    while (angle < 0)
        angle += 360;
    // Check over 360 angle
    while (angle >= 360)
        angle -= 360;

    setLEDColor(number, getHalfSine((angle+120)%360),
                getHalfSine(angle),
                getHalfSine((angle+240)%360));
}

// Send colors to the strip and show them. Disables interrupts while processing.
void showStrip_old(){
    //__bic_SR_register(GIE);         // disable interrupts
    __dint();

    // send RGB color for every LED
    uint16_t i;
    uint8_t j;

    for (i = 0; i < WS2812_COUNT; ++i){
        uint8_t rgb[3] = {ws2812_leds[i].green, ws2812_leds[i].red, ws2812_leds[i].blue}; // get RGB color for this LED

        // send green, then red, then blue
        for (j = 0; j < 3; ++j){
            uint8_t mask = 0x80;                 // b1000000

            // check each of the 8 bits
            while(mask != 0){
                while (!(UCB0IFG & UCTXIFG));    // wait to transmit

                if (rgb[j] & mask){             // most significant bit first
                    UCB0TXBUF = HIGH_CODE;      // send 1
                }
                else {
                    UCB0TXBUF = LOW_CODE;       // send 0
                }

                mask >>= 1;                     // check next bit
            }
        }
    }

    // send RES code for at least 50 us (1250 cycles at 25 MHz)
    __delay_cycles(1250);

    //__bis_SR_register(GIE);         // enable interrupts
    __eint();
}

void showStrip(){
    //__bic_SR_register(GIE);         // disable interrupts
    __dint();
    
    // send RGB color for every LED
    uint16_t i;
    uint8_t j;

    for (i = 0; i < WS2812_COUNT; ++i){
        uint8_t rgb[3] = {ws2812_leds[i].green, ws2812_leds[i].red, ws2812_leds[i].blue}; // get RGB color for this LED

        // send green, then red, then blue
        for (j = 0; j < 3; ++j){
            uint8_t mask = 0x80;                 // b1000000

            // check each of the 8 bits
            while(mask != 0){


                if (rgb[j] & mask){             // most significant bit first
                    P3OUT |= OUTPUT_PIN;
                    __delay_cycles(30);
                    P3OUT &= ~OUTPUT_PIN;
                    __delay_cycles(2);

                }
                else {
                    P3OUT |= OUTPUT_PIN;
                    __delay_cycles(3);
                    P3OUT &= ~OUTPUT_PIN;
                    __delay_cycles(13);
                }

                mask >>= 1;                     // check next bit
            }
        }
    }

    // send RES code for at least 50 us (1250 cycles at 25 MHz)
    __delay_cycles(1250);

    //__bis_SR_register(GIE);         // enable interrupts
    __eint();
}

// Push all the colors of LED's forward (to next LED) and load the new color in the first LED
void advanceStrip(uint8_t r, uint8_t g, uint8_t b){
    int16_t i;

    for (i=WS2812_COUNT-2; i>=0; i--){
        // Copy all data one step forward
        ws2812_leds[i + 1].red = ws2812_leds[i].red;
        ws2812_leds[i + 1].green = ws2812_leds[i].green;
        ws2812_leds[i + 1].blue = ws2812_leds[i].blue;
    }
    // Load new data in first pos
    ws2812_leds[0].red = r;
    ws2812_leds[0].green = g;
    ws2812_leds[0].blue = b;

    showStrip();  // Refresh strip
}

// Clear the color of all LEDs (make them black/off)
void clearStrip(){
    fillStrip(0x00, 0x00, 0x00);    // black (all off)
}

// Fill the strip with a solid color. This will update the strip.
void fillStrip(uint8_t r, uint8_t g, uint8_t b){
    uint16_t i;
    for (i = 0; i < WS2812_COUNT; ++i){
        setLEDColor(i, r, g, b);        // set all LEDs to specified color
    }
    showStrip();                        // refresh strip
}
