/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


#define TMR2PERIOD ((80000000 / 256) / 10) //100 ms
#if TMR2PERIOD > 0xffff
#error "TimerPeriodIsTooBig"
#endif

int mytime = 0x0000;
int timeoutcount = 0;
int prime = 1234567;
volatile int *porte = (volatile int *) 0xbf886110;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  timeoutcount++;

  if (timeoutcount == 10){
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
//display_image(96, icon);
    tick( &mytime );
    timeoutcount = 0;
  }
  // clearing flag
  IFSCLR(0) = 0x100;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  	// Set *E to address of TRISE.
	volatile int *E = (volatile int *) 0xbf886100;
	
	// Set last 8 bits to zero, i.e. sets them as output pins.
	*E = *E & 0xff00;

	// Initialize port D, set bits 11-5 as inputs.
	// !If wrong order, try 0x07f0.!
	TRISD = TRISD & 0x0fe0;

	PR2 = TMR2PERIOD;    //set timeperiod 
	T2CONSET = 0x70;         //setting prescaling to 1:256
	TMR2 = 0;		   //reset timer
	T2CONSET = 0x8000;    //starting timer, set fit bit 15 to 1

	IPC(2) = IPC(2) | 0x10;
  	// set bit no 8 to enable interupt
  	IEC(0) = 0x100;
  	// calling interupt from labwork.S
  	enable_interrupt();

	return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
   prime = nextprime( prime );
 display_string( 0, itoaconv( prime ) );
 display_update();
			
}		
