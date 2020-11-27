
The MCU has 3 different timers. Depending on the way you have designed your apps, you can use all of them simultaneously for different uses in you app - debounce checks, 3 sec checks, seconds/minutes countdown etc. You can also have different checks use the same timer as long as the checks do not have to be done simultaneously.

The way to configure them is identical to what is covered in the 2 lectures on timers and interrupts.

The timer configuration bits can be changed in the following register (x = 1,2 and 3 for timer 1,2 and 3 respectively):

TxCON  

The timer interrupt routine for the timer is (x = 1,2 and 3 for timer 1,2 and 3 respectively):

void __attribute__((interrupt, no_auto_psv)) _TxInterrupt(void){}

The interrupt configuration bits for each timers are:

    IFS0bits.TxIF = 0;   // Interrupt flag, x = 1,2 and 3 for timer 1,2 and 3 respectively
    IEC0bits.TxIE = 1;   // To enable timer interrupts, x = 1,2 and 3 for timer 1,2 and 3 respectively

The interrupt priority bits for each timers are. If you do not set the priority, by default all timers get the highest priority if enabled:

    IPC0bits.T1IP = 1;  // any value between 0 and 7 for timer 1 priority
    IPC1bits.T2IP = 1;  // any value between 0 and 7 for timer 2 priority
    IPC2bits.T3IP = 1;  // any value between 0 and 7 for timer 3 priority

Hope that makes designing your apps easier.

Dr. V