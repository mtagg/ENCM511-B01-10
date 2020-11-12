/* 
 * File:   MACROS.h
 * Author: mmmta
 *
 * Created on November 1, 2020, 7:52 PM
 */

#ifndef MACROS_H
#define	MACROS_H

#ifdef	__cplusplus
extern "C" {
#endif


    
//useful macros:
#define LED_ON      LATBbits.LATB8 = 1  
#define LED_OFF     LATBbits.LATB8 = 0
#define LED_TOGGLE  LATBbits.LATB8 = ~LATBbits.LATB8
#define PB1 PORTAbits.RA4
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA2


#ifdef	__cplusplus
}
#endif

#endif	/* MACROS_H */

