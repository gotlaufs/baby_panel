#include "animations.h"
#include "main.h"
#include "defines.h"

void lin_pot_bar_1(void){
    // This results in noisy binary data
    led_data.bar_1 = input_data.lin_pot >> 2;
}

void big_round_pot_bar_2(void){
    if (((~input_data.big_round_pot & 0xFFF) >> 2) > 930){
        led_data.bar_2 = 0x200;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 837){
        led_data.bar_2 = 0x100;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 744){
        led_data.bar_2 = 0x080;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 651){
        led_data.bar_2 = 0x040;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 558){
        led_data.bar_2 = 0x020;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 465){
        led_data.bar_2 = 0x010;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 372){
        led_data.bar_2 = 0x008;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 279){
        led_data.bar_2 = 0x004;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 186){
        led_data.bar_2 = 0x002;
    }
    else if (((~input_data.big_round_pot & 0xFFF) >> 2) > 93){
        led_data.bar_2 = 0x001;
    }
    else{
        led_data.bar_2 = 0x000;
    }
}

void end_sw_1_groups(void){
    static uint8_t end_sw_2_counter = 0;

    switch (end_sw_2_counter){
    case 0:
        led_data.square_blue = ON;
        led_data.square_red = ON;
        led_data.square_white = ON;
        break;
    case 1:
        led_data.square_blue = OFF;
        led_data.square_red = OFF;
        led_data.square_white = OFF;

        led_data.rgb_square_1.r = ON;
        led_data.rgb_square_1.g = ON;
        led_data.rgb_square_1.b = ON;
        led_data.rgb_square_2.r = ON;
        led_data.rgb_square_2.g = ON;
        led_data.rgb_square_2.b = ON;
        led_data.rgb_square_3.r = ON;
        led_data.rgb_square_3.g = ON;
        led_data.rgb_square_3.b = ON;
        break;
    case 2:
        led_data.rgb_square_1.r = OFF;
        led_data.rgb_square_1.g = OFF;
        led_data.rgb_square_1.b = OFF;
        led_data.rgb_square_2.r = OFF;
        led_data.rgb_square_2.g = OFF;
        led_data.rgb_square_2.b = OFF;
        led_data.rgb_square_3.r = OFF;
        led_data.rgb_square_3.g = OFF;
        led_data.rgb_square_3.b = OFF;

        led_data.seven_seg_1 = 0xFF;
        led_data.seven_seg_2 = 0xFF;
        break;
    case 3:
        led_data.seven_seg_1 = 0x00;
        led_data.seven_seg_2 = 0x00;

        led_data.big_green = ON;
        led_data.big_red = ON;
        led_data.big_orange = ON;
        break;
    case 4:
        led_data.big_green = OFF;
        led_data.big_red = OFF;
        led_data.big_orange = OFF;
        break;
    }

    end_sw_2_counter++;

    if (end_sw_2_counter > 4){
        end_sw_2_counter = 0;
    }
}

void end_sw_2_big_round(void){
    static uint8_t end_sw_1_counter = 0;

    switch (end_sw_1_counter){
    case 0:
        led_data.big_red = ON;
        break;
    case 1:
        led_data.big_red = OFF;
        break;
    case 2:
        led_data.big_green = ON;
        break;
    case 3:
        led_data.big_green = OFF;
        break;
    case 4:
        led_data.big_orange = ON;
        break;
    case 5:
        led_data.big_orange = OFF;
        break;
    case 6:
        led_data.big_red = ON;
        led_data.big_green = ON;
        break;
    case 7:
        led_data.big_red = OFF;
        led_data.big_green = OFF;
        break;
    case 8:
        led_data.big_orange = ON;
        led_data.big_green = ON;
        break;
    case 9:
        led_data.big_orange = OFF;
        led_data.big_green = OFF;
        break;
    case 10:
        led_data.big_red = ON;
        led_data.big_orange = ON;
        break;
    case 11:
        led_data.big_red = OFF;
        led_data.big_orange = OFF;
        break;
    case 12:
        led_data.big_red = ON;
        led_data.big_green = ON;
        led_data.big_orange = ON;
        break;
    case 13:
        led_data.big_red = OFF;
        led_data.big_green = OFF;
        led_data.big_orange = OFF;
        break;

    }
    end_sw_1_counter++;
    if (end_sw_1_counter > 13){
        end_sw_1_counter = 0;
    }
}

void tall_blue_square_blue(void){
    if (led_data.square_blue){
        led_data.square_blue = OFF;
    }
    else{
        led_data.square_blue = ON;
    }
}

void tall_grey_square_white(void){
    if (led_data.square_red){
        led_data.square_red = OFF;
    }
    else{
        led_data.square_red = ON;
    }
}

void tall_red_square_red(void){
    if (led_data.square_white){
        led_data.square_white = OFF;
    }
    else{
        led_data.square_white = ON;
    }
}

void small_round_pot_orange_dimmable(void){
    led_data.orange_1.change = (input_data.small_round_pot >> 2) -
            led_data.orange_1.b;
    led_data.orange_2.change = (input_data.small_round_pot >> 2) -
                        led_data.orange_2.b;
    led_data.orange_3.change = (input_data.small_round_pot >> 2) -
                        led_data.orange_3.b;
}

void blue_tact_square_rgb(void){
    static uint8_t blue_tact_counter = 0;

    switch (blue_tact_counter){
    case 0:
        led_data.rgb_square_1.r = ON;
        break;
    case 1:
        led_data.rgb_square_1.r = OFF;
        led_data.rgb_square_2.g = ON;
        break;
    case 2:
        led_data.rgb_square_2.g = OFF;
        led_data.rgb_square_3.b = ON;
        break;
    case 3:
        led_data.rgb_square_3.b = OFF;
        led_data.rgb_square_1.r = ON;
        led_data.rgb_square_1.g = ON;
        break;
    case 4:
        led_data.rgb_square_1.r = OFF;
        led_data.rgb_square_1.g = OFF;
        led_data.rgb_square_2.g = ON;
        led_data.rgb_square_2.b = ON;
        break;
    case 5:
        led_data.rgb_square_2.g = OFF;
        led_data.rgb_square_2.b = OFF;
        led_data.rgb_square_3.b = ON;
        led_data.rgb_square_3.r = ON;
        break;
    case 6:
        led_data.rgb_square_1.r = ON;
        led_data.rgb_square_1.g = ON;
        led_data.rgb_square_1.b = ON;
        led_data.rgb_square_2.r = ON;
        led_data.rgb_square_2.g = ON;
        led_data.rgb_square_2.b = ON;
        led_data.rgb_square_3.r = ON;
        led_data.rgb_square_3.g = ON;
        led_data.rgb_square_3.b = ON;
        break;
    case 7:
        led_data.rgb_square_1.r = OFF;
        led_data.rgb_square_1.g = OFF;
        led_data.rgb_square_1.b = OFF;
        led_data.rgb_square_2.r = OFF;
        led_data.rgb_square_2.g = OFF;
        led_data.rgb_square_2.b = OFF;
        led_data.rgb_square_3.r = OFF;
        led_data.rgb_square_3.g = OFF;
        led_data.rgb_square_3.b = OFF;
        break;
    }
    blue_tact_counter++;

    if (blue_tact_counter > 7){
        blue_tact_counter = 0;
    }
}
