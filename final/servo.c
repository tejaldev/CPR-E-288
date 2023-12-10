#include "Timer.h"
#include "lcd.h"
#include "servo.h"
#include <stdint.h>
#include <stdbool.h>
#include "button.h"

extern volatile int right_calibration_val;
extern volatile int left_calibration_val;

void servo_init(void) {
    int pulse_width = 320000;
    int midValue = 24000;
    SYSCTL_RCGCTIMER_R |= 0x2; //Enable Timer 1
    SYSCTL_RCGCGPIO_R |= 0x2;  //Enable GPIO Port B clock
    GPIO_PORTB_AFSEL_R |= 0x20; //PB5
    GPIO_PORTB_PCTL_R |= 0x700000;
    GPIO_PORTB_DEN_R |= 0x20; //Enable PB5
    GPIO_PORTB_DIR_R |= 0x20; //Set PB5 to output	

    TIMER1_CTL_R &= ~0x100; //Disable Timer 1
    TIMER1_CFG_R |= 0x4;    //16 bit
    TIMER1_TBMR_R |= 0b1010;    //Periodic, edge count, PWM
    TIMER1_TBMR_R &= 0b1010;
    //TIMER1_CTL_R &= 0xBFFF;
    TIMER1_TBPR_R = 0x4;
    TIMER1_TBILR_R = 0x4E200;
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xC000;
    TIMER1_CTL_R |= 0x100;  //Enable Timer 1B
}

void servo_move(float degrees) {
    /*
    int move = (degrees * 88.889) + 16000;
    TIMER1_TBPMR_R |= ((320000 - move) >> 16);
    TIMER1_TBMATCHR_R = (move & 0xFFFF);
    */
    int right_value = 51937;
    int left_value = 24716;   //((right_value - left_value) / 180)
    if(degrees <= 180 && degrees >= 0){
        int angle = (-1 * degrees) + 180;
        int move = left_value + (angle * ((right_value - left_value) / 180));
        TIMER1_TBPMR_R |= ((320000 - move) >> 16);
        TIMER1_TBMATCHR_R = (move & 0xFFFF);
        timer_waitMillis(50);
    }
}


void calibrate_move(float degrees) {
    /*
    int move = (degrees * 88.889) + 16000;
    TIMER1_TBPMR_R |= ((320000 - move) >> 16);
    TIMER1_TBMATCHR_R = (move & 0xFFFF);
    */
    int right_value = 51285;
    int left_value = 21782;   //((right_value - left_value) / 180)
    int angle = (-1 * degrees) + 180;
    int move = left_value + (angle * ((right_value - left_value) / 180));
    TIMER1_TBPMR_R |= ((320000 - move) >> 16);
    TIMER1_TBMATCHR_R = (move & 0xFFFF);
    timer_waitMillis(200);
}


void calibrate(){
    int pulse_width = 320000;
    int angle = 90;
    int direction = 1;
    int pressed = 0;
    int position_counter;
    servo_move(angle);

    while(1){
        pressed = button_getButton();
        position_counter = TIMER1_TBMATCHR_R;
        //move 1 degree
        if(pressed == 1){
            angle += 1 * direction;

        }
        //move 5 degrees
        if(pressed == 2){
            angle += 5 * direction;
        }
        //switch between clockwise and counter
        if(pressed == 3){
            direction *= -1;
            timer_waitMillis(100);
        }
        //if clockwise, move to zero. if counter, move to 180
        if(pressed == 4){
            if(direction == 1){
                angle = 180;
            }

            if(direction == -1){
                angle = 0;
            }
        }

        calibrate_move(angle);
        position_counter = TIMER1_TBMATCHR_R;
        if(direction == 1){
            lcd_printf("angle incrementing, angle = %d position = %d",angle, position_counter);
        }
        if(direction == -1){
            lcd_printf("angle decrementing, angle = %d position = %d",angle, position_counter);
        }

    }

}
