/*
*
*   uart_extra_help.c
* Description: This is file is meant for those that would like a little
*              extra help with formatting their code, and followig the Datasheet.
*/

#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include<inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#define REPLACE_ME 0x00
volatile  char uart_data;
volatile  char flag;


void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0x02;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0x02;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R = 0x03;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   = 0x03;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   = 0x02;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz] 
    float brd;
    int    fbrd;
    int    ibrd;


    brd = 16000000.0/(16.0 * baud); // page 903
    ibrd = (int) brd;
    fbrd = ((brd - ibrd) * 64 + 0.5);

    UART1_CTL_R &= ~(UART_CTL_UARTEN);      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;        // write fractional portion of BRD to FBRD
    UART1_LCRH_R = UART_LCRH_WLEN_8;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   = UART_CC_CS_SYSCLK;      // use system clock as clock source (page 939)
    UART1_CTL_R |= UART_CTL_UARTEN;      // enable UART1

}


void uart_sendChar(char data)
{


   //wait here as long as the FIFO is Full
   while(UART1_FR_R & 0x20)
   {
   }

   //send data
   UART1_DR_R = data;

}



char uart_receive(void)
{

   char data = 0;

   // keep waiting as long as FIFO is empty
   while(UART1_FR_R & UART_FR_RXFE)
   {
   }

   //mask the 4 error bits and grab only 8 data bits
   data = (char)(UART1_DR_R & 0xFF); 
   return data;

}




void uart_sendStr(const char *data)
{

    int i=0;
    //Iterates over string and uses sendChar
    for(i=0; data[i] != '\0'; i++)
    {
        uart_sendChar(data[i]);
    }

}


// _PART3


void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    UART1_IM_R |= 0x10; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9  (bit 6)
    // Note: NVIC register descriptions are found in chapter 3.4
    NVIC_EN0_R |= 0b01000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{
// STEP1: Check the Masked Interrupt Status

    if(UART_MIS_RXMIS){
        flag = '1';
    }

//STEP2:  Copy the data 
    uart_data = (char)(UART1_DR_R & 0xFF);

//STEP3:  Clear the interrupt
    UART1_ICR_R = 0xFF;

}

/*
void printString(char chars[]){
    char send[30];
    sprintf(send, chars);
    int i;
    for(i=0;i<strlen(send);i++){
        uart_sendChar(send[i]);
    }
*/

