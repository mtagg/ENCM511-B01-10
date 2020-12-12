#include "pulse.h"
extern unsigned int STATE;
unsigned int capture1; //capture1:2 used for TMR2 value capturing for pulse measurement
unsigned int capture2;

void TimerInit(void){
    
//T2 setup:
    T2CONbits.T32 = 1;          //32bit timer t2
    TMR2 = 0x0000;              //clear TMR2
    T2CONbits.TON = 1;          //timer ON initially
    IEC0bits.T2IE = 0;          //disable t2 interrupts
    T2CONbits.TSIDL = 0;        //idle operation enabled
    T2CONbits.TCS = 0;          //base clock

//Interrupt Control Setup (pin14):
    IC1CONbits.ICSIDL = 0;      // continue operation in idle mode
    IFS0bits.IC1IF = 0;         //reset IC flag
    IEC0bits.IC1IE = 1;         //enable IC interrupts
    IPC0 = IPC0 | 0x0070;       //IC interrupt priority
    
//Capture parameters:
    IC1CONbits.ICTMR = 1;       // TMR2 contents saved upon capture event
    IC1CONbits.ICI = 0b11;      // interrupt every 4th capture event (only interrupt once per measurement)
    IC1CONbits.ICM = 0b011;     //capture every rising edge (only when put into idle)  
    Idle();                     //Idle to collecting Timer data    
    
    return;
}

char* frequency(void){

//check base clock speed:
    int base = 32;                                  // default osc value is 32khz
    if      (OSCCONbits.COSC == 0x0) base = 8000;   // current oscillator is 8MHz
    else if (OSCCONbits.COSC == 0x6) base = 500;    // current osc is 500kHz

//calculate pulse under test
    double ncycles = 2*(capture2-capture1); //number of base clock cycles per 4 rising edges(3full pulse periods)
    double f = (base*3)/ncycles;
   
//put result into a string
    static char buff[6];
    sprintf(buff, "%4.1f", f);
    return buff;
}

char* amplitude(void){
    unsigned int max = 0x0;
    unsigned int min = 0x3ff;
    unsigned int ctr = 0;
    unsigned int tmp = 0;

//collect # of pulse samples @ pin 15
    while (ctr++ < 100){                
        tmp = do_ADC();                 //new sample at pin15
        if (tmp < min) min = tmp;       //update min
        if (tmp > max) max = tmp;       //update max
    }
//calculate amplitude:
    double amp;                         
    amp = (3.25*(max+min));             
    amp/=2048;

//convert amplitude to string & return:
    static char buff[4];                
    sprintf(buff, "%1.2f V", amp);
    return buff;
}



void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void){
    if (IFS0bits.IC1IF == 1){
        capture1 = IC1BUF;          //save buffer value
        capture2 = TMR2;            //save final TMR2 value
        IC1CONbits.ICM = 0b000;     //disable the ICM
        T2CONbits.TON = 0;          //disable t2  not needed anymore
        IFS0bits.IC1IF = 0;         //reset interrupt flag  
    }
}