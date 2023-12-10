#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "senddata.h"

void sendData(float distance, int degrees) {
    int i;
    int j;
    char degs[5];
    char dist[5];

    // turns ints and floats into char arrays
    for (i = 0; i <= 5; i++) {
         sprintf(degs, "%d", degrees);
    }

    for (i = 0; i < 5; i++) {
         sprintf(dist, "%0.0f", distance);
    }

    //sends each char in the array to PuTTy
    for(j = 0; j < strlen(degs); j++) {
        uart_sendChar(degs[j]);
    }

    uart_sendChar('\t');

    for(j = 0; j < strlen(dist); j++) {
        uart_sendChar(dist[j]);
    }

    uart_sendChar('\n');
    uart_sendChar('\r');

}
