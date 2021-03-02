#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void) {

	return (PORTD >> 8) & 0x000F;  //shifting right by 8 bits to get bits from 8 to 11 and masking 4 lsb
}

int getbtns(void) {

	return (PORTD >> 5) & 0x0007;   //shifting right by 5 bits to get bits 5 to 7 and masking by 7 to get 3 lsb
}