/*
 * Panel with various lights for babies
 */

#include "msp430.h"
#include "stdint.h"

#include "main.h"
#include "animations.h"
#include "defines.h"
#include "hc595_bb.h"
#include "ws2812.h"

void form_hc595_bytes(void);
void update_dimmable_leds(void);
void update_direct_leds(void);

void SetVcoreUp (unsigned int level);
void init_clk(void);
void init_pins(void);
void poll_inputs(void);

void investigate_leds(void);
void investigate_hc585(void);

enum {
    NORMAL = 0,
    PANEL_OFF = 1
} DISPLAY_MODE = NORMAL;

volatile struct led_data_struct led_data;
volatile struct input_data_struct input_data;
volatile struct hc595_data_struct hc595;

volatile uint8_t hc595_data[HC595_COUNT];

int main(){
	WDTCTL = WDTPW + WDTHOLD;

	// Set up MCLK @25 MHz
	init_clk();

    init_pins();

	// Set up SPI module for WS2812 LED strips
	initStrip();

	// Set up timers for PWM LEDs
	TA0CCR0 = 1023;
	TA2CCR0 = 1023;
	//TA2CCR0 = 1023;

    TA0CTL |= TASSEL_2 + ID_3 + TACLR + MC_1;
    TA2CTL |= TASSEL_2 + ID_3 + TACLR + MC_1;
    //TA2CTL |= TASSEL_2 + ID_3 + TACLR + MC_1;

    TA0CCTL3 = OUTMOD_7;		// Orange LED 1
    TA0CCTL4 = OUTMOD_7;		// Orange LED 2
    TA2CCTL1 = OUTMOD_7;		// Orange LED 3
    //TA2CCTL1 = OUTMOD_7;	// Unused
    //TA2CCTL2 = OUTMOD_7;	// Unused

	// Set up ADC channels for reading potentiometers
    ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_15; // Turn on ADC12, extend sampling time
                                                  // to avoid overflow of results
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_3;         // Use sampling timer, repeated sequence
    ADC12MCTL0 = ADC12INCH_3;                     // ref+=AVcc, channel = A3
    ADC12MCTL1 = ADC12INCH_4;                     // ref+=AVcc, channel = A4
    ADC12MCTL2 = ADC12INCH_5;                     // ref+=AVcc, channel = A5
    ADC12MCTL3 = ADC12INCH_6 + ADC12EOS;          // ref+=AVcc, channel = A6, end seq.
    ADC12IE = ADC12IE3;                           // Enable ADC12 memory 3 interrupt
    ADC12CTL0 |= ADC12ENC;                        // Enable conversions
    ADC12CTL0 |= ADC12SC;                         // Start convn - software trigger

	// Set up global structures
	// HC595 shift register
	hc595.data		= hc595_data;
	hc595.len		= HC595_COUNT;
	hc595.reg_port	= &P6OUT;
	hc595.rclk_pin	= BIT1;
	hc595.ser_pin	= BIT2;
	hc595.srclk_pin = BIT0;

	// Dimmable LEDs
	led_data.orange_1.b			= 0;
	led_data.orange_1.change	= 0;
	led_data.orange_1.led_reg	= &TA0CCR3;
	led_data.orange_2.b			= 0;
	led_data.orange_2.change	= 0;
	led_data.orange_2.led_reg	= &TA0CCR4;
	led_data.orange_3.b			= 0;
	led_data.orange_3.change	= 0;
	led_data.orange_3.led_reg	= &TA2CCR1;

	__eint();

	// Main loop
	// Animation variables
	int8_t seven_seg_counter_1 = 0, seven_seg_counter_2 = 9;
	uint8_t ws2812_phase = 0;
	uint8_t ws2812_max = 100;
	int ws2812_r = ws2812_max;
	int ws2812_g = 0;
	int ws2812_b = 0;

	enum {
	    STD_POT,
	    SWITCH
	} per_led_control = STD_POT;

	int per_led_frame = 0;
	uint8_t per_led_counter_max = 50;
	uint8_t per_led_counter = per_led_counter_max;

	while(1){
	    // Debug HW
	    //investigate_leds();
	    //investigate_hc585();
	    if (input_data.power){
	        DISPLAY_MODE = NORMAL;
	    }
	    else{
	        DISPLAY_MODE = PANEL_OFF;
	    }

	    if (DISPLAY_MODE == NORMAL){
	        // Update Bar graph 1 according to linear pot
	        lin_pot_bar_1();

	        // Update bar graph 2 according to big round pot
	        big_round_pot_bar_2();

	        // Handle end switch 1
	        if (input_data.end_sw_1){
	            end_sw_1_groups();
	            input_data.end_sw_1 = OFF;
	        }

	        // Handle end switch 2
	        if (input_data.end_sw_2){
	            end_sw_2_big_round();
	            input_data.end_sw_2 = OFF;
	        }

	        // Handle tall tact switches
	        if (input_data.big_tact_blue){
	            tall_blue_square_blue();
	            input_data.big_tact_blue = OFF;
	        }
            if (input_data.big_tact_red){
                tall_grey_square_white();
                input_data.big_tact_red = OFF;
            }
            if (input_data.big_tact_grey){
                tall_red_square_red();
                input_data.big_tact_grey = OFF;
            }

            // Handle pot with dimmable orange LEDs
            small_round_pot_orange_dimmable();

            // Handle small blue tact and RGB square LEDs
            if (input_data.blue_tact){
                blue_tact_square_rgb();
                input_data.blue_tact = OFF;
            }

            // Handle grey tact switches - responsible for 7-seg displays
            if (input_data.grey_1){
                led_data.seven_seg_1 = seg_lookup[seven_seg_counter_1];
                input_data.grey_1 = OFF;

                seven_seg_counter_1++;

                if (seven_seg_counter_1 > 9){
                    seven_seg_counter_1 = 0;
                }
            }

            if (input_data.grey_2){
                led_data.seven_seg_2 = seg_lookup[seven_seg_counter_2];

                input_data.grey_2 = OFF;

                seven_seg_counter_2--;

                if (seven_seg_counter_2 < 0){
                    seven_seg_counter_2 = 9;
                }
            }

            // Handle std pot to drive the perimeter LEDs
            if (per_led_control == STD_POT){
            if ((input_data.std_pot >> 2) > 1015){
                led_data.per_leds = 0x00;
            }
            else if ((input_data.std_pot >> 2) > 960){
                led_data.per_leds = 0x01;
            }
            else if ((input_data.std_pot >> 2) > 918){
                led_data.per_leds = 0x03;
            }
            else if ((input_data.std_pot >> 2) > 862){
                led_data.per_leds = 0x07;
            }
            else if ((input_data.std_pot >> 2) > 800){
                led_data.per_leds = 0x0F;
            }
            else if ((input_data.std_pot >> 2) > 560){
                led_data.per_leds = 0x1F;
            }
            else if ((input_data.std_pot >> 2) > 220){
                led_data.per_leds = 0x3F;
            }
            else if ((input_data.std_pot >> 2) > 10){
                led_data.per_leds = 0x7F;
            }
            else{
                led_data.per_leds = 0xFF;
            }
            }

            // Handle toggle switch
            if (input_data.blue_toggle == MIDDLE){
                per_led_control = STD_POT;
            }
            else{
                per_led_control = SWITCH;
                per_led_counter--;
                if (per_led_counter == 0){
                    per_led_counter = per_led_counter_max;
                    if (input_data.blue_toggle == LEFT){
                        per_led_frame--;
                    }
                    else{
                        per_led_frame++;
                    }
                }

                if (per_led_frame > 7){
                    per_led_frame = 0;
                }
                if (per_led_frame < 0){
                    per_led_frame = 7;
                }

                led_data.per_leds = 0x01 << per_led_frame;
            }

            if (input_data.red_toggle){
                switch (ws2812_phase){
                case 0:
                    // Inrement r, decrement b
                    ws2812_r++;
                    ws2812_b--;

                    if ((ws2812_r > ws2812_max) || (ws2812_b < 0)){
                        ws2812_phase = 1;
                        ws2812_r = ws2812_max;
                        ws2812_b = 0;
                    }
                    break;
                case 1:
                    // Increment g, decrement r
                    ws2812_g++;
                    ws2812_r--;

                    if ((ws2812_g > ws2812_max) || (ws2812_r < 0)){
                        ws2812_phase = 2;
                        ws2812_g = ws2812_max;
                        ws2812_r = 0;
                    }
                    break;
                case 2:
                    // Increment b, decrement g
                    ws2812_b++;
                    ws2812_g--;

                    if ((ws2812_b > ws2812_max) || (ws2812_g < 0)){
                        ws2812_phase = 0;
                        ws2812_b = ws2812_max;
                        ws2812_g = 0;
                    }
                    break;
                }

                fillStrip(ws2812_r, ws2812_g, ws2812_b);
            }
            else{
                fillStrip(0, 0, 0);
            }

	    }

	    else if (DISPLAY_MODE == PANEL_OFF){
	        led_data.bar_1 = 0x000;
	        led_data.bar_2 = 0x000;
	        led_data.big_green = OFF;
	        led_data.big_orange = OFF;
	        led_data.big_red = OFF;
	        led_data.orange_1.change = -2000;
	        led_data.orange_2.change = -2000;
	        led_data.orange_3.change = -2000;
	        led_data.per_leds = 0x00;
	        led_data.rgb_square_1.r = OFF;
	        led_data.rgb_square_1.g = OFF;
	        led_data.rgb_square_1.b = OFF;
            led_data.rgb_square_2.r = OFF;
            led_data.rgb_square_2.g = OFF;
            led_data.rgb_square_2.b = OFF;
            led_data.rgb_square_3.r = OFF;
            led_data.rgb_square_3.g = OFF;
            led_data.rgb_square_3.b = OFF;
	        led_data.seven_seg_1 = 0x00;
	        led_data.seven_seg_2 = 0x00;
	        led_data.square_blue = OFF;
	        led_data.square_red = OFF;
	        led_data.square_white = OFF;

	        fillStrip(0, 0, 0);

	    }

	    poll_inputs();
	    form_hc595_bytes();
		showStrip();	// ws2812
		hc595_write_data(&hc595);
		update_dimmable_leds();
		update_direct_leds();


		//__delay_cycles(1000);
		__delay_cycles(120000);

	}
}

/*
 * Helper functions
 */

void investigate_leds(void){
#define BLINK_DELAY 12000000
    // Flash all LEDs connected to LaunchPad directly
    P3OUT |= BIT3 | BIT4 | BIT2 | BIT5 | BIT6 | BIT1;
    P4OUT |= BIT1 | BIT2;
    P7OUT |= BIT0 | BIT4;
    P8OUT |= BIT1;

    while(1){
        // Loop start, blink something 3 times
        P8OUT &= ~BIT1; // Right RGB B
        __delay_cycles(BLINK_DELAY);
        P8OUT |= BIT1;
        __delay_cycles(BLINK_DELAY);

        P8OUT &= ~BIT1;
        __delay_cycles(BLINK_DELAY);
        P8OUT |= BIT1;
        __delay_cycles(BLINK_DELAY);

        P8OUT &= ~BIT1;
        __delay_cycles(BLINK_DELAY);
        P8OUT |= BIT1;
        __delay_cycles(BLINK_DELAY);


        P7OUT &= ~BIT0; // Left RGB R
        __delay_cycles(BLINK_DELAY);
        P7OUT |= BIT0;
        __delay_cycles(BLINK_DELAY);

        P7OUT &= ~BIT4; // Right RGB R
        __delay_cycles(BLINK_DELAY);
        P7OUT |= BIT4;
        __delay_cycles(BLINK_DELAY);


        P4OUT &= ~BIT1; // Middle RGB B
        __delay_cycles(BLINK_DELAY);
        P4OUT |= BIT1;
        __delay_cycles(BLINK_DELAY);

        P4OUT &= ~BIT2; // Middle RGB G
        __delay_cycles(BLINK_DELAY);
        P4OUT |= BIT2;
        __delay_cycles(BLINK_DELAY);

        // 123456
        P3OUT &= ~BIT1; // Right RGB G
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT1;
        __delay_cycles(BLINK_DELAY);

        P3OUT &= ~BIT2; // Middle RGB R
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT2;
        __delay_cycles(BLINK_DELAY);

        P3OUT &= ~BIT3; // Perimeter 0 (Green) Active high
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT3;
        __delay_cycles(BLINK_DELAY);

        P3OUT &= ~BIT4; // Perimeter 1 (Blue) Active High
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT4;
        __delay_cycles(BLINK_DELAY);

        P3OUT &= ~BIT5; // Left RGB B
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT5;
        __delay_cycles(BLINK_DELAY);

        P3OUT &= ~BIT6; // Left RGB G
        __delay_cycles(BLINK_DELAY);
        P3OUT |= BIT6;
        __delay_cycles(BLINK_DELAY);

        __delay_cycles(BLINK_DELAY);
    }
#undef BLINK_DELAY
}

void update_direct_leds(void){
    // Update LEDs that are directly connected to LaunchPad
    // First reset everything
    P8OUT |= BIT1;
    P7OUT |= BIT0 | BIT4;
    P4OUT |= BIT1 | BIT2;
    P3OUT |= BIT1 | BIT2 | BIT5 | BIT6;
    P3OUT &= ~(BIT3 | BIT4);

    P3OUT |= (led_data.per_leds & BIT0) ? BIT3 : 0;
    P3OUT |= (led_data.per_leds & BIT1) ? BIT4 : 0;

    P3OUT &= (led_data.rgb_square_3.g) ? ~BIT1 : 0xFF;
    P3OUT &= (led_data.rgb_square_2.r) ? ~BIT2 : 0xFF;
    P3OUT &= (led_data.rgb_square_1.b) ? ~BIT5 : 0xFF;
    P3OUT &= (led_data.rgb_square_1.g) ? ~BIT6 : 0xFF;

    P4OUT &= (led_data.rgb_square_2.b) ? ~BIT1 : 0xFF;
    P4OUT &= (led_data.rgb_square_2.g) ? ~BIT2 : 0xFF;

    P7OUT &= (led_data.rgb_square_1.r) ? ~BIT0 : 0xFF;
    P7OUT &= (led_data.rgb_square_3.r) ? ~BIT4 : 0xFF;

    P8OUT &= (led_data.rgb_square_3.b) ? ~BIT1 : 0xFF;

}

void investigate_hc585(void){
    static int counter = 0;
    static int current = 5;

    switch (counter){
    case 0:
        hc595_data[current] = 0x01;
        break;
    case 1:
        hc595_data[current] = 0x00;
        break;
    case 2:
        hc595_data[current] = 0x01;
        break;
    case 3:
        hc595_data[current] = 0x00;
        break;
    case 4:
        hc595_data[current] = 0x02;
        break;
    case 5:
        hc595_data[current] = 0x04;
        break;
    case 6:
        hc595_data[current] = 0x08;
        break;
    case 7:
        hc595_data[current] = 0x10;
        break;
    case 8:
        hc595_data[current] = 0x20;
        break;
    case 9:
        hc595_data[current] = 0x40;
        break;
    case 10:
        hc595_data[current] = 0x80;
        break;


    }

    counter++;

    if (counter > 10){
        counter = 0;
    }

}

void init_clk(void){
	// From MSP430F5529 code example
	// TODO: Check if this works and CLK is @25Mhz
	// TODO: Output SMCLK == MCLK to a pin, to check MCLK freq.
	// No pin for MCLK pin on LaunchPad
	SetVcoreUp (0x01);
	SetVcoreUp (0x02);
	SetVcoreUp (0x03);		// Vcore must be incremented one step at a time
	UCSCTL3 = SELREF_2;                       // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_7;                      // Select DCO range 50MHz operation
	UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 25MHz
	                                          // (N + 1) * FLLRef = Fdco
	                                          // (762 + 1) * 32768 = 25MHz
	                                          // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
	__delay_cycles(782000);

	// Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
	do
	{
	  UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
	                                          // Clear XT2,XT1,DCO fault flags
	  SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	}while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

}

void SetVcoreUp (unsigned int level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}

/*
 * Form the list of bytes for sending to HC595 shift reg
 */
void form_hc595_bytes(void){
	uint16_t i;

	// Clear all the bytes first
	for (i=0; i<HC595_COUNT; i++){
		hc595_data[i] = 0x00;
	}
	hc595_data[3] = 0xFF;   // 7-seg displays are common anode
	hc595_data[4] = 0xFF;

	// Update the values in the data buffer
	hc595_data[0] |= (led_data.bar_1 & BIT2) ? BIT0 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT3) ? BIT1 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT5) ? BIT2 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT4) ? BIT3 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT7) ? BIT4 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT6) ? BIT5 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT8) ? BIT6 : 0;
	hc595_data[0] |= (led_data.bar_1 & BIT9) ? BIT7 : 0;

    hc595_data[1] |= (led_data.bar_2 & BIT5) ? BIT0 : 0;
    hc595_data[1] |= (led_data.bar_2 & BIT4) ? BIT1 : 0;
    hc595_data[1] |= (led_data.bar_2 & BIT2) ? BIT2 : 0;
    hc595_data[1] |= (led_data.bar_2 & BIT3) ? BIT3 : 0;
    hc595_data[1] |= (led_data.bar_2 & BIT1) ? BIT4 : 0;
    hc595_data[1] |= (led_data.bar_2 & BIT0) ? BIT5 : 0;
    hc595_data[1] |= (led_data.bar_1 & BIT0) ? BIT6 : 0;
    hc595_data[1] |= (led_data.bar_1 & BIT1) ? BIT7 : 0;

    hc595_data[2] |= (led_data.square_red) ? BIT0 : 0;
    hc595_data[2] |= (led_data.square_white) ? BIT1 : 0;
    hc595_data[2] |= (led_data.square_blue) ? BIT2 : 0;
    hc595_data[2] |= (led_data.per_leds & BIT2) ? BIT3 : 0;
    hc595_data[2] |= (led_data.bar_2 & BIT9) ? BIT4 : 0;
    hc595_data[2] |= (led_data.bar_2 & BIT8) ? BIT5 : 0;
    hc595_data[2] |= (led_data.bar_2 & BIT7) ? BIT6 : 0;
    hc595_data[2] |= (led_data.bar_2 & BIT6) ? BIT7 : 0;
    // 7-seg:
    // [dp gfedcba]
    hc595_data[3] &= (led_data.seven_seg_2 & BIT7) ? ~BIT0 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT2) ? ~BIT1 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT3) ? ~BIT2 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT4) ? ~BIT3 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT0) ? ~BIT4 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT1) ? ~BIT5 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT5) ? ~BIT6 : 0xFF;
    hc595_data[3] &= (led_data.seven_seg_2 & BIT6) ? ~BIT7 : 0xFF;

    hc595_data[4] &= (led_data.seven_seg_1 & BIT7) ? ~BIT0 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT2) ? ~BIT1 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT3) ? ~BIT2 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT4) ? ~BIT3 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT0) ? ~BIT4 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT1) ? ~BIT5 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT5) ? ~BIT6 : 0xFF;
    hc595_data[4] &= (led_data.seven_seg_1 & BIT6) ? ~BIT7 : 0xFF;

    hc595_data[5] |= (led_data.per_leds & BIT7) ? BIT0 : 0;
    hc595_data[5] |= (led_data.per_leds & BIT6) ? BIT1 : 0;
    hc595_data[5] |= (led_data.big_orange) ? BIT2 : 0;
    hc595_data[5] |= (led_data.big_green) ? BIT3 : 0;
    hc595_data[5] |= (led_data.big_red) ? BIT4 : 0;
    hc595_data[5] |= (led_data.per_leds & BIT5) ? BIT5 : 0;
    hc595_data[5] |= (led_data.per_leds & BIT4) ? BIT6 : 0;
    hc595_data[5] |= (led_data.per_leds & BIT3) ? BIT7 : 0;
}

void update_dimmable_leds(void){
    int new_value = 0;
    static int change_speed = 10;

    if (led_data.orange_1.change > 0){
        new_value = led_data.orange_1.b + change_speed;
        if (new_value > 1023){
            new_value = 1023;
        }
        led_data.orange_1.change -= change_speed;

        led_data.orange_1.b = new_value;
    }
    if (led_data.orange_1.change < 0){
        new_value = led_data.orange_1.b - change_speed;
        if (new_value < 0){
            new_value = 0;
        }
        led_data.orange_1.change += change_speed;

        led_data.orange_1.b = new_value;
    }


    if (led_data.orange_2.change > 0){
        new_value = led_data.orange_2.b + change_speed;
        if (new_value > 1023){
            new_value = 1023;
        }
        led_data.orange_2.change -= change_speed;

        led_data.orange_2.b = new_value;
    }
    if (led_data.orange_2.change < 0){
        new_value = led_data.orange_2.b - change_speed;
        if (new_value < 0){
            new_value = 0;
        }
        led_data.orange_2.change += change_speed;

        led_data.orange_2.b = new_value;
    }


    if (led_data.orange_3.change > 0){
        new_value = led_data.orange_3.b + change_speed;
        if (new_value > 1023){
            new_value = 1023;
        }
        led_data.orange_3.change -= change_speed;

        led_data.orange_3.b = new_value;
    }
    if (led_data.orange_3.change < 0){
        new_value = led_data.orange_3.b - change_speed;
        if (new_value < 0){
            new_value = 0;
        }
        led_data.orange_3.change += change_speed;

        led_data.orange_3.b = new_value;
    }


    *led_data.orange_1.led_reg = led_data.orange_1.b;
    *led_data.orange_2.led_reg = led_data.orange_2.b;
    *led_data.orange_3.led_reg = led_data.orange_3.b;

}

void init_pins(void){
    // Input switches:
    // Input, Interrupt enable, high to low, internal pullup enabled
    P1DIR &= ~(BIT6 | BIT3 | BIT2);
    P2DIR &= ~(BIT7 | BIT0 | BIT2 | BIT6 | BIT3);
    P3DIR &= ~(BIT7);
    P4DIR &= ~(BIT0 | BIT3);
    P8DIR &= ~(BIT2);

    P1IES |= BIT6 | BIT3 | BIT2;
    P2IES |= BIT7 | BIT0 | BIT2 | BIT6 | BIT3;

    P1REN |= BIT6 | BIT3 | BIT2;
    P2REN |= BIT7 | BIT0 | BIT2 | BIT6 | BIT3;
    P3REN |= BIT7;
    P4REN |= BIT0 | BIT3;
    P8REN |= BIT2;

    P1OUT |= BIT6 | BIT3 | BIT2;
    P2OUT |= BIT7 | BIT0 | BIT2 | BIT6 | BIT3;
    P3OUT |= BIT7;
    P4OUT |= BIT0 | BIT3;
    P8OUT |= BIT2;

    P1IFG &= ~(BIT6 | BIT3 | BIT2);
    P2IFG &= ~(BIT7 | BIT0 | BIT2 | BIT6 | BIT3);

    P1IE |= BIT6 | BIT3 | BIT2;
    P2IE |= BIT7 | BIT0 | BIT2 | BIT6 | BIT3;

    // Set up pins for HC595 shift register in bit-bang mode
    P6DIR |= BIT0 | BIT1 | BIT2;
    //P6DS |= BIT0 | BIT1 | BIT2;
    P6OUT &= ~(BIT0 | BIT1 | BIT2);

    // Set up pins for reading analog voltage
    P6DIR &= ~(BIT3 | BIT4 | BIT5 | BIT6);
    P6SEL |= BIT3 | BIT4 | BIT5 | BIT6;

    // Set up LED output pins
    // Output, high drive strength
    P3DIR |= BIT3 | BIT4 | BIT2 | BIT5 | BIT6 | BIT1;
    P4DIR |= BIT1 | BIT2;
    P7DIR |= BIT0 | BIT4;
    P8DIR |= BIT1;

    P3DS |= BIT3 | BIT4 | BIT2 | BIT5 | BIT6 | BIT1;
    P4DS |= BIT1 | BIT2;
    P7DS |= BIT0 | BIT4;
    P8DS |= BIT1;

    // Most of directly connected pins are active low
    P3OUT &= ~(BIT3 | BIT4);
    P3OUT |= BIT2 | BIT5 | BIT6 | BIT1;
    P4OUT |= BIT1 | BIT2;
    P7OUT |= BIT0 | BIT4;
    P8OUT |= BIT1;

    // Set up Timer pins for dimmable LEDs
    P2DIR |= BIT4;
    P1DIR |= BIT5 | BIT4;
    P2SEL |= BIT4;
    P1SEL |= BIT5 | BIT4;

}

void poll_inputs(void){
    // Poll the pins that do not ave interrupts
    // These are all HW pins wired to some switches
    if (~(P3IN) & BIT7){
        // Toggle Right
        input_data.blue_toggle = RIGHT;
    }
    else if (~(P4IN) & BIT0){
        // Toggle left
        input_data.blue_toggle = LEFT;
    }
    else{
        input_data.blue_toggle = MIDDLE;
    }

    if (~(P4IN) & BIT3){
        // Red toggle
        input_data.red_toggle = ON;
    }
    else{
        input_data.red_toggle = OFF;
    }

    if (~(P8IN) & BIT2){
        // On/Off
        input_data.power = ON;
    }
    else{
        input_data.power = OFF;
    }
}

/*
 * ISRs
 */

// GPIOs
__attribute__((interrupt(PORT1_VECTOR)))
void port_1_handler(void){
    //
    __nop();
    if (P1IFG & BIT2){
        // Small blue tact
        input_data.blue_tact = ON;
        P1IFG &= ~BIT2;
    }

    if (P1IFG & BIT3){
        // Tall blue tact
        input_data.big_tact_blue = ON;
        P1IFG &= ~BIT3;
    }

    if (P1IFG & BIT6){
        // Big reed switch 1 (by the side)
        input_data.end_sw_1 = ON;
        P1IFG &= ~BIT6;
    }
}

__attribute__((interrupt(PORT2_VECTOR)))
void port_2_handler(void){
    //
    __nop();
    if (P2IFG & BIT0){
        // Grey tact left
        input_data.grey_1 = ON;
        P2IFG &= ~BIT0;
    }

    if (P2IFG & BIT2){
        // Grey tact right
        input_data.grey_2 = ON;
        P2IFG &= ~BIT2;
    }

    if (P2IFG & BIT3){
        // Tall grey tact switch
        input_data.big_tact_grey = ON;
        P2IFG &= ~BIT3;
    }

    if (P2IFG & BIT6){
        // Tall red tact switch
        input_data.big_tact_red = ON;
        P2IFG &= ~BIT6;
    }

    if (P2IFG & BIT7){
        // Big reed switch 2 (towards middle)
        input_data.end_sw_2 = ON;
        P2IFG &= ~BIT7;
    }
}

__attribute__((interrupt(ADC12_VECTOR)))
void adc_handler(void){
    switch(__even_in_range(ADC12IV,34)){
    case 12:
        // ADCIFG on Memory ch 3
        input_data.std_pot = ADC12MEM3;
        input_data.big_round_pot = ADC12MEM0;
        input_data.lin_pot = ADC12MEM1;
        input_data.small_round_pot = ADC12MEM2;
    }
}

