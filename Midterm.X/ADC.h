
#ifndef ADC_H
#define	ADC_H

#include <xc.h>
#include "UART2.h"
#include "pulse.h"
#include <stdio.h>
#include <math.h>
    
    void ADCinit(void);
    unsigned int do_ADC(void);
    void ADC_Display(void);
    void ADC2mV(unsigned int V);
    void ADC2ohm(unsigned int V);
    void clearLine(void);


#endif	/* ADC_H */

