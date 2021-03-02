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

int mytime = 0x5957;
int timeoutcount = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
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

	return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile int *porte = (volatile int *) 0xbf886110;

	(*porte) += 0x1;

	int sw = getsw();
	int btn = getbtns();
	/* 
	Checks if bit 1(001) is pressed and 2(010) and so on ..	
	sw is a number 0-f, shift it into the right position and OR it with the correct zeroed byte of mytime.
	*/	

	if (btn & 1) {
		mytime = (sw << 4) | (mytime & 0xff0f);
	}
	if (btn & 2) {
		mytime = (sw << 8) | (mytime & 0xf0ff);
	}
	if (btn & 4) {
		mytime = (sw << 12) | (mytime & 0x0fff);
	}
	//removed delay
	if (IFS(0) & 0x100) {

		IFSCLR(0) = 0x100;	//reset t2 interrupt flag
		timeoutcount++;

		if (timeoutcount == 10) {

		time2string( textstring, mytime );
		display_string( 3, textstring );
		display_update();
		tick( &mytime );
		display_image(200, icon);
				
		timeoutcount = 0;		
		}
	}
			
         }		
