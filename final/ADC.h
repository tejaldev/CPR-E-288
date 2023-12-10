/*
*
*   ADC.h
*
*/

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>


void ADC_init(void);

int ADC_read(void);

/*
 * returns a refined ir distance from 50 samples
 */
float ADC_getDistance();


#endif /* ADC_H_ */
