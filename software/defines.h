/*
 * Various structs and defines
 */
#ifndef _DEFINES_H_
#define _DEFINES_H_

#define HC595_COUNT		6
#define WS2812_COUNT	7

/*
 * Pin defines
 */
// LEDs
#define PIN_ORANGE_1	BIT4	// Port 1
#define PIN_ORANGE_2	BIT5	// Port 1
#define PIN_ORANGE_3	BIT0	// Port 2

// HC595 pins
#define PIN_RCLK	BIT0	// Port 6
#define PIN_SRCLK	BIT1	// Port 6
#define PIN_SER		BIT2	// Port 6

// Potentiometers - analog inputs
#define PIN_POT_SMALL	BIT3	// Port 6
#define PIN_POT_BIG		BIT4	// Port 6
#define PIN_POT_STD		BIT5	// Port 6
#define PIN_POT_LIN		BIT6	// Port 6

// Buttons/switches and everything
#define PIN_END_SW_1		BIT6	// Port 1
#define PIN_END_SW_2		BIT7	// Port 2
#define PIN_GREY_1			BIT6	// Port 2
#define PIN_GREY_2			BIT3	// Port 2
#define PIN_BLUE_TACT		BIT3	// Port 1
#define PIN_BIG_TACT_BLUE	BIT2	// Port 1
#define PIN_BIG_TACT_RED	BIT3	// Port 4
#define PIN_BIG_TACT_GREY	BIT0	// Port 4
#define PIN_RED_TOGGLE		BIT7	// Port 3
#define PIN_BLUE_TOGGLE_L	BIT2	// Port 8
#define PIN_BLUE_TOGGLE_R	BIT2	// Port 2	// Shared with SMCLK OUT


/*
 * 7-seg display lookup table
 *
 *	bit[7..0] = [0gfedcba]
 *
 *	TODO: Finish this
 */
extern const uint8_t seg_lookup[12];


enum io_status_enum{
	OFF = 0,
	ON = 1
};

// Dimmable LED struct
// For use with a timer compare register
struct dimmable_led{
	volatile uint16_t b;			// Current brightness
	volatile unsigned int  *led_reg;	// Timer register
	volatile int change;		// How many steps of brightness to change
};

// Non dimmable RGB light
struct rgb_struct{
	enum io_status_enum r;
	enum io_status_enum g;
	enum io_status_enum b;
};


// LED data structure
struct led_data_struct{
	volatile enum io_status_enum big_green;	 // Big green 220 V LED
	volatile enum io_status_enum big_red;	 // Big red 220 V LED
	volatile enum io_status_enum big_orange; // Big orange 220 V LED
	volatile uint8_t seven_seg_1; 			 // 7-segment display
	volatile uint8_t seven_seg_2;			 // 7-segment display
	volatile struct dimmable_led orange_1;	 // Dimmable huge round orange LED
	volatile struct dimmable_led orange_2; 	 // Dimmable huge round orange LED
	volatile struct dimmable_led orange_3;	 // Dimmable huge round orange LED
	volatile struct rgb_struct rgb_square_1; // Square RGB LED
	volatile struct rgb_struct rgb_square_2; // Square RGB LED
	volatile struct rgb_struct rgb_square_3; // Square RGB LED
	volatile enum io_status_enum square_red; // Huge square red LED
	volatile enum io_status_enum square_white;	// Huge square white LED
	volatile enum io_status_enum square_blue;	// Huge square white LED
	volatile uint16_t bar_1;			// 10-segment bar indicator
	volatile uint16_t bar_2;			// 10-segment bar indicator
	volatile uint8_t per_leds;			// 8 random color LEDs around the box perimeter
};

// input data
enum toggle_sw{
	MIDDLE = 0,	// Or Off
	LEFT = 1,
	RIGHT = 2
};


struct input_data_struct{
    volatile enum io_status_enum power;     // Green Power switch
	volatile enum toggle_sw blue_toggle;	// Blue 2-position toggle switch
	volatile enum io_status_enum end_sw_1;	// Micro-switch with huge reed
	volatile enum io_status_enum end_sw_2;	// Micro-switch with huge reed
	volatile enum io_status_enum grey_1;	// Square grey tact switch
	volatile enum io_status_enum grey_2;	// Square grey tact switch
	volatile uint16_t big_round_pot;	// Big round pot
	volatile enum io_status_enum blue_tact;	// Dark blue tactile button
	volatile enum io_status_enum big_tact_blue;	// Tall tactile switch
	volatile enum io_status_enum big_tact_red;	// Tall tactile switch
	volatile enum io_status_enum big_tact_grey;	// Tall tactile switch
	volatile enum io_status_enum red_toggle;	// Red toggle switch
	volatile uint16_t std_pot;	// Average jelly-bean pot
	volatile uint16_t lin_pot;	// Linear potentiometer
	volatile uint16_t small_round_pot;// Smaller round potentiometer
};

#endif // _DEFINES_H_
