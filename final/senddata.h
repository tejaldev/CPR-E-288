#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void sendData(float distance, int degrees);
