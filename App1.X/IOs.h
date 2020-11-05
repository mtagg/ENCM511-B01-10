#ifndef IOSTUFF_H
#define IOSTUFF_H

void IOinit(void);
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif	/* IOSTUFF_H */

