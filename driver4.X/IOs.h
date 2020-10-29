#ifndef IOSTUFF_H
#define IOSTUFF_H
//useful macros:
#define LED_ON      {LATBbits.LATB8 = 1;}    
#define LED_OFF     {LATBbits.LATB8 = 0;}
#define LED_TOGGLE  {LATBbits.LATB8 = ~LATBbits.LATB8;}
#define PB1 PORTAbits.RA4
#define PB2 PORTBbits.RB4
#define PB3 PORTAbits.RA2
#define ALL3BUTTONS PORTAbits.RA4 + PORTBbits.RB4 + PORTAbits.RA2
void IOinit(void);
void IOcheck(void);
#endif	/* IOSTUFF_H */

