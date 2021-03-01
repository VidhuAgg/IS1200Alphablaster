#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
	int ret = 0x0;
	ret = ret | (PORTD >> 8);
	ret = ret & 0xf;
	return (ret);
}
int getbtns(void)
{
	int ret = 0x0;
	ret = ret | (PORTD >> 5);
	ret = ret & 0x7;
	return (ret);
}