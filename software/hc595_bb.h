/*
 * SN74HC595 bit-bang handler
 *
 * The chip is a serial to parallel shift register
 */
#ifndef _HC595_BB_H_
#define _HC595_BB_H_

#include "stdint.h"

#define HC595_DELAY 500

struct hc595_data_struct{
	volatile uint8_t *data;
	uint8_t len;

	volatile uint8_t *reg_port;
	uint8_t rclk_pin;
	uint8_t srclk_pin;
	uint8_t ser_pin;
};

/*
 * Write a number of bytes bit-bang style to HC595 shift register
 */
void hc595_write_data(volatile struct hc595_data_struct *data);

#endif // _HC595_BB_H_
