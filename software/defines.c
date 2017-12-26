/*
 * Various structs and defines
 */

#include "stdint.h"
#include "defines.h"

// [dp gfedcba]
const uint8_t seg_lookup[12] = {
		0x3F,	// 0
		0x06,	// 1
		0x5B,	// 2
		0x4F,	// 3
		0x66,	// 4
		0x6D,	// 5
		0x7D,	// 6
		0x07,	// 7
		0x7F,	// 8
		0x6F,	// 9
		0x00,	// All off
		0xFF	// All on
};
