
#include "msp430.h"
#include "stdint.h"

#include "hc595_bb.h"

void hc595_write_data(volatile struct hc595_data_struct *data){
	uint16_t i;
	uint8_t byte_mask;
	// Make sure clock and strobe lines are low
	*data->reg_port &= ~(data->rclk_pin + data->srclk_pin);
	__delay_cycles(HC595_DELAY);

	for (i=0; i<data->len; i++){
		// For each bit
		byte_mask = 0x01;
		while (byte_mask){
			if (byte_mask & data->data[i]){
				*data->reg_port |= data->ser_pin;
				__delay_cycles(HC595_DELAY);
			}
			else{
				*data->reg_port &= ~data->ser_pin;
				__delay_cycles(HC595_DELAY);
			}

			// Make positive edge on SRCLK
			*data->reg_port |=  data->srclk_pin;
			__delay_cycles(HC595_DELAY);
			*data->reg_port &= ~data->srclk_pin;
			__delay_cycles(HC595_DELAY);

			byte_mask = byte_mask << 1;
		}
	}

	// Strobe once to copy data to output register
	__delay_cycles(HC595_DELAY);
	*data->reg_port |=  data->rclk_pin;
	__delay_cycles(HC595_DELAY);
	*data->reg_port &= ~data->rclk_pin;
}
