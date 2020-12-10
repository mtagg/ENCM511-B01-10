
#ifndef ADC_H
#define	ADC_H

#include <xc.h>
#include "UART2.h"
#include "ChangeClk.h"
#include "pulse.h"
#include <stdio.h>
#include <math.h>
#define PB1 PORTAbits.RA4
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA2

    
    void ADCinit(void);
    unsigned int do_ADC(void);
    void ADC_Display(void);
    char* ADC2mV(unsigned int V);
    char* ADC2ohm(unsigned int V);
    void clear(void);

#endif	/* ADC_H */

