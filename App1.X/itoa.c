#include "itoa.h"

 char* itoa(uint8_t NUMBER){
     uint8_t n = 3;              // 2bytes to hold a string that represents a 2 digit decimal
     static char str[3];         

     str[2] = '\0';              //end the char array with NULL
     do {                                     // do-while loop to make sure that 0s arent forgotten
         str[--n] = (NUMBER % 10 + '0');      // ie: if number is 32, the char before null will be 2  
         NUMBER /= 10;                         // ie: get rid of the least significant digit and restart
     }while (n>=0);                           // stop when the char array is finished
       
     return str;                             //returns a char array in correct order   
 }