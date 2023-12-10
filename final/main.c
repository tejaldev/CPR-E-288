

/**
 * main.c
 */

#define _RESET 0
#define _PART0 0 // calibration of sensors/servo
#define _PART1 1 //part 1 is the actual part for the demo

#include "math.h"
#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include"open_interface.h"
#include "movement.h"
#include "ADC.h"
#include "servo.h"
#include "ping.h"
#include "senddata.h"
#include "missioncommands.h"

int main() {
#if _RESET
    //resetSimulationBoard();
    oi_close();
#else
    /*
        don't forget that 90 degrees is facing straight forward
        i.e. 0 degrees is 90 right, 180 degrees is 90 left
    */

    timer_init();
    lcd_init();
    lcd_clear();
    uart_init(115200);
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    ADC_init();
    servo_init();
    ping_init();

    #if _PART0

    int i;
    int averageIR = 0;
    float averagePing = 0;
    float distVal;
    char sendStr[30];
    for(i=0;i<10;i++){
        cyBOT_Scan(90, &scan);
        averageIR += scan.IR_raw_val;
        averagePing += scan.sound_dist;
    }
    averagePing = averagePing / 10;
    averageIR = averageIR / 10;
    //distVal = (float)(60000000.0*pow(averageIR, -1.383)); //function converting voltage to centimeters
    uart_sendChar('\r');
    uart_sendChar('\n');
    sprintf(sendStr, "%d", averageIR);
    sendString(sendStr);
    uart_sendChar('\r');
    uart_sendChar('\n');
    sprintf(sendStr, "%.2f", averagePing);
    sendString(sendStr);

    return 0;

    #endif


#if _PART1 // detect objects within a certain distance threshold and return the angle and distance of all objects detected

    char instruction[] = "section 3, project group 2 demo in progress";
    uart_sendStr(instruction);
    while(1){
        move_cybot();
    }


#endif

#endif
}
