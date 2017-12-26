/*
 * Animation functions
 *
 * Meant to be called at about 100 Hz
 */
#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

// Update bar 1 with binary data from linear pot
void lin_pot_bar_1(void);

// Update bar 2 with data from the Big Round pot
void big_round_pot_bar_2(void);

// Update all LEDs in groups with data from end switch 1
void end_sw_1_groups(void);

// Update big round LEDs with signals from end switch 2
void end_sw_2_big_round(void);

// Update big square LEDs with data from three tall tact switches
void tall_blue_square_blue(void);
void tall_grey_square_white(void);
void tall_red_square_red(void);

// Update dimmable Orange LEDs with data from small round pot
void small_round_pot_orange_dimmable(void);

// Update square RGB LEDs with signals from blue tact switch
void blue_tact_square_rgb(void);

#endif // _ANIMATIONS_H_
