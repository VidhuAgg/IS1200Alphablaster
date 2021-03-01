/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 
   This file modified 2018-02-xx by Oscar Eklund and Vilhelm Elofsson
   For copyright and licensing, see file COPYING */
#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdlib.h>
#include "standard.h" /*includes the use of functions rand()*/
#include<math.h>

volatile int * porte = (volatile int *) 0xbf886110;
int timecount = 0;
int spawnflag = 0;
int pos = 0;
int spawn2 = 0;
int scoreCount = 0;
int score = 0;
struct Astroid{
   int x;
   int y;
};

struct Ship{
    int x;
    int y;   
};

struct bullet{
    int x;
    int y;
};

struct ammobox{
    int x;
    int y;
};

struct Ship Alpha;
struct Astroid ast[14];
struct ammobox box;
int count1 = 0;
int count2=0;
int ammo = 10;
int counter = 0;
struct bullet b[10];
int start = 1;
int diffauto = 0;

char Start1[] = "Choose";
char Start2[] = "Difficulty";
char Start3[] = "L-Start, R-Diff";
int difficulty = 1;
char difficultyShown[1] = "1";

char gameOver[] = "Your Score:";
char gameOver2[] = "High Score:";
int gameovercheck = 0;
char buffer[5];
char buffer2[5]; 


char clear[] = " ";
void moveup()
{
    if(Alpha.y > 0)
    {
        Alpha.y --;
    } 
}
/* Lab-specific initialization goes here */
void labinit(void)
{
	volatile int* trise = (volatile int *) 0xbf886100; // pointer to TRISE address
	*trise = *trise & 0xff00; // set bits 7-0 of PORT E to outputs
	*porte = 0x0; // initialize to 0
	TRISDSET = 0xfe0; // set bit 11-5 of PORT D as output
	
	PR2 = 31250;
	T2CON = 0x70; // sets prescale to 256
	TMR2 = 0;
	T2CONSET = 0x8000; // set bit 15 to 1 (enable timer)
	IEC(0) = 0x100;
	IPC(2) = 0x7;
	enable_interrupt();
	return;
}
/* This function is called repetitively from the main program */


void fire()
{
  
    b[counter].x = 0;
    b[counter].y = Alpha.y;
    counter++;
    if(counter > 9)
    {
        counter = 0;
    }
    ammo--;
}



void movedown()
{
    if(Alpha.y < 3)
    {
        Alpha.y ++;
    }
}
int HighScore()
{
    if(count1>count2)
    {
        count2 = count1;
    }
    return count2;
}
void screen_refresh()
{
    int j;
    for(j=0;j<512;j++)
    {
        Screen[j] = Screen_Refresh[j];
    }
}
void ScreenUpdate()
{
    int i;
    int i2;
    int i3;
    int j;
    int j1;
    int t;
    screen_refresh();
    for(i=0;i<10;i++)
    {
        if(b[i].x>=0 && b[i].x<14){
            for(i2=0;i2<8;i2++)
            { 
                Screen[16+(8*b[i].x)+i2+128*b[i].y] = bullet[i2];//bullet image;
            }
        }
    }
    int t1;
    for(t1=0;t1<14;t1++)
    {   
        if(ast[t1].x>=0 && ast[t1].x<14){
            for(t=0; t< 8; t++){
                Screen[16+(8*ast[t1].x)+t+128*ast[t1].y] = astroid[t];//ast image;
            }
        }
    }
    for(i3 = 0; i3 < 16; i3++){
        Screen[i3 + (128 * Alpha.y)] = Ship[i3];//image ship
    }
    if(box.x>0 && box.x<14){
        for(j1=0;j1<8;j1++)
        {
           Screen[16+(8*box.x)+j1+box.y*128] = Supply[j1];
        }
    }
}

void bulletcol()
{
   int i;
   int j;
   for(j=0 ; j<10; j++)
   {

       
        if(b[j].x== box.x && b[j].y == box.y)
        {
                ammo = ammo + 5;
                box.x = 28;
                box.y = rand() % 4;
                b[j].x = -1;
                if(ammo>10)
                {
                     ammo = 10;
                }  
        }

        for(i = 0; i < 14; i++)
        {
            if((b[j].x == ast[i].x) && b[j].y == ast[i].y)
            {
            
                ast[i].x = ast[i].x + 13;
                ast[i].y = rand() % 4;
                b[j].x = -1;
            }
        }
       
    
    

    }



}
void moveright()
{

   int j;
   for(j = 0; j<10; j++)
   {
      if(b[j].x >= 0 && b[j].x<14)
        {
            b[j].x++;
        }

   }
   bulletcol();
}

void moveleft()
{
    int i;
    for(i = 0; i<14; i++)
    {
        if(ast[i].x<=0)
        {
            ast[i].x = 14;
            ast[i].y = rand() % 4;
        }
        ast[i].x--;
    }
    if(box.x<=0)
    {
        box.x = 28;
        box.y = rand() % 4;
    }
    box.x--;
    bulletcol();
}

void Spawn()
{
    int i;
    counter = 0;
    diffauto = 0;
    ammo = 10;
    int k = 14;
    for(i = 0; i<14; i++)
    {
        ast[i].x = k++;
        ast[i].y = (rand() % 4); 
    }
    Alpha.x = 0;
    Alpha.y = 0;
    box.x = 28;
    box.y = (rand() % 4);
    int p;
    for (p=0; p<10; p++)
    {
        b[p].x = -1;
        b[p].y = 0;
    }
}

int Astroidcollison()
{
    int i;
    for(i = 0;i <14; i++)
    {
        if(ast[i].x == 0 && ast[i].y == Alpha.y)
        {
           	return 1;
        }
    }
    return 0;
}

void ScoreUp()
{
  count1 = count1 + difficulty;
}


void labwork(void) {
	switch(getbtns()) { 
		case 0x04  :
			if(Alpha.y > 0){ // move car up 1 pos if pushed
				moveup();
				delay( 150 );
			}
			
			break;
		case 0x02  :
			if(Alpha.y < 3){ // move car down 1 pos if pushed
				movedown();
				delay( 150 );
			}
			break;
		case 0x01  :
                        if (ammo > 0)
                        {
                                fire();
        		        delay (150);
                        }
                        break;
	}	
}

/* Interrupt Service Routine */
void user_isr(void) 
{
	if((IFS(0) & 0x0100))
    {
		IFS(0) = IFS(0) & 0xfffffeff;
		timecount++;
		spawnflag++;                   
		
		while (gameovercheck)
               {                               // function for when car has collided, gameover
			T2CONCLR = 0x8000;
			screen_refresh();
                        sprintf(buffer,"%d",count1);
			display_string( 0, gameOver );
			display_string( 1, buffer );
                        sprintf(buffer2,"%d",count2);
                        display_string( 2, gameOver2 );
                        display_string( 3, buffer2 );
			display_update();	
			if(getbtns() == 0x04)
                        {                       // restart if button pushed
			
				spawnflag = 0;
				start = 1;
				gameovercheck = 0;
				delay(300);
				T2CONSET = 0x8000;
			}
		}
		
        while(start) 
        {                         // function for start, difficulty select
            count1= 0;
            T2CONCLR = 0x8000;
            screen_refresh();
            difficultyShown[0] = difficulty + '0';
            
            display_string( 0, Start1 );
            display_string( 1, Start2 );
            display_string( 2, difficultyShown);
            display_string( 3, Start3 );
            display_update();	
            gameovercheck = 0;

            if(getbtns() == 0x04)
            {
                screen_refresh();
                Spawn();
                ScreenUpdate();
                display_image(0,Screen);
                start = 0;
                T2CONSET = 0x8000;
                break;
            }
		
            if(getbtns() == 0x02)
            {
                if(difficulty < 4)
                {
                    difficulty++;
                }
                else
                {
                    difficulty = 1;
                }
                delay( 175 );
            }
        
	}
         
        if(diffauto == 40)
        {
            difficulty++;
            diffauto = 0;
        }
            
        if(spawnflag == 6/difficulty)
        {                                  // determines how fast objects move and spawn
            moveright();
            moveleft();
                               //giving minor bugs at times missing target
            spawnflag = 0;
            ScreenUpdate();
            display_image(0,Screen);
            gameovercheck = Astroidcollison();             
            diffauto++;
            ScoreUp();
            HighScore();
            
            
        }
    }	
	
}
