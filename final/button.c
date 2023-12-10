/*
 * button.c
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 *  @edit: Phillip Jone 05/30/2019: Mearged Spring 2019 version with Fall 2018
 */
 


//The buttons are on PORTE 3:0
// GPIO_PORTE_DATA_R -- Name of the memory mapped register for GPIO Port E, 
// which is connected to the push buttons
#include "button.h"
#include "lcd.h"
#include "timer.h"
#include "uart.h"


#define MIN(x,y) (((x) < (y)) ? (x): (y))
#define MAX(x,y)(((x) > (y)) ? (x) : (y))


#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80


#define BUTTON_PORT     GPIO_PORTE_DATA_R

//global variables
volatile int button_event;
volatile int button_num;


/**
 * Initialize PORTE and configure bits 0-3 to be used as inputs for the buttons.
 */
void button_init() {
    static uint8_t initialized = 0;


        //Check if already initialized
        if(initialized)
            return;


        //Turn on PORTE system clock
        SYSCTL_RCGCGPIO_R |= BIT4;


        //Set the buttons to inputs and enable
        GPIO_PORTE_DIR_R &= ~(BIT6 - 1); //Clear bits 5:0
        GPIO_PORTE_DEN_R |= (BIT6 - 1);


        initialized = 1;

}



/**
 * Initialize and configure PORTE interupts
 */
void init_button_interrupts() {

    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TODO: Complete code below

    // 1) Mask the bits for pins 0-3
    GPIO_PORTE_IM_R &= ~0b11110000;

    // 2) Set pins 0-3 to use edge sensing
    GPIO_PORTE_IS_R &= ~0b11110000;

    // 3) Set pins 0-3 to use both edges. We want to update the LCD
    //    when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0b00001111;

    // 4) Clear the interrupts
    GPIO_PORTE_ICR_R = 0b11111111;

    // 5) Unmask the bits for pins 0-3
    GPIO_PORTE_IM_R |= 0b00001111;

    // TODO: Complete code below
    // 6) Enable GPIO port E interrupt
    NVIC_EN0_R |= 0b00010000;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * prints a message with the given number of button included
 */
void print_button(int pressed_button){

    char phrase[25];
    sprintf(phrase, "Button %d is pressed!", pressed_button);
    lcd_printf(phrase);

}



/**
 * sends a message to putty with the pressed button
 */
/*void putty_print(int pressed_button){
    char phrase[25];
    sprintf(phrase, "Button %d is pressed!", pressed_button);
    int i;
    int j = strlen(phrase);
    for(i = 0; i < j; i++){
        cyBot_sendByte(phrase[i]);
    }
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');

}*/


/**
 * Interrupt handler -- executes when a GPIO PortE hardware event occurs (i.e., for this lab a button is pressed)
 */
void gpioe_handler() {


    button_event = 1;
    button_num = button_getButton();

    //print_button(button_num);

    // Clear interrupt status register
    GPIO_PORTE_ICR_R = 0xFF;


}







/**
 * Returns the position of the rightmost button being pushed.
 * @return the position of the righttmost button being pushed. 4 is the rightmost button, 1 is the leftmost button.  0 indicates no button being pressed
 */
uint8_t button_getButton() {


    int pressed_button = 0;


    if(!((GPIO_PORTE_DATA_R)&1)){
        pressed_button = MAX(1,pressed_button);
    }


    if (!((GPIO_PORTE_DATA_R >> 1)&1)){
        pressed_button = MAX(2, pressed_button);
    }


    if (!((GPIO_PORTE_DATA_R >> 2)&1)){
        pressed_button = MAX(3, pressed_button);
    }


    if (!((GPIO_PORTE_DATA_R >> 3)&1)){
        pressed_button = MAX(4, pressed_button);
    }

    return pressed_button;


}








