/*
 * button.h
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton
 *
 * @edit: Phillip Jones 05/30/2019 : Removed uneeded helper functions
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <inc/tm4c123gh6pm.h>


//initialize the push buttons
void button_init();


///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
uint8_t button_getButton();

/**
 * Initialize and configure PORTE interupts
 */
void init_button_interrupts();



/**
 * Interrupt handler -- executes when a GPIO PortE hardware event occurs (i.e., for this lab a button is pressed)
 */
void gpioe_handler();



/**
 * prints a message with the given number of button included
 */
void print_button(int pressed_button);

/**
 * sends a message to putty with the pressed button
 */
void putty_print(int pressed_button);


#endif /* BUTTON_H_ */
