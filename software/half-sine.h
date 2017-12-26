#include <stdint.h>
/*
 * half-sine.h
 *
 *  Created on: 24.07.2016.
 *      Author: Roberts Gotlaufs
 *
 *  Half-rectified sine wave lookup table.
 *  Sligtly optimized (from 360 to 240 values).

 *  Original:
 *  http://www.instructables.com/id/How-to-Make-Proper-Rainbow-and-Random-Colors-With-/step2/Rainbow-time/
 *
 */

#ifndef HALF_SINE_H_
#define HALF_SINE_H_

extern const uint8_t h_sine[240];

// Get value of half-rectified sine from lookup table
uint8_t getHalfSine(const int16_t angle);

#endif /* HALF_SINE_H_ */
