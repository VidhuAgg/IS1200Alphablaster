/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 
   This file modified 2018-02-xx by Oscar Eklund and Vilhelm Elofsson
   This file modified 2021-03-06 by Vidhu Aggarwal and Yash Dhanore
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
int name1 = 0;
int letterpos =0;
int Highscore[12];

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
int ammo = 10;
int counter = 0;
struct bullet b[10];
int start = 1;
int diffauto = 0;
int health;
int anim = 0;
int test = 0;
int anim1 = 127;

char Start4[] = "bt3 - Highscores";
char Start1[] = "Alpha-Blaster2.0";
char Start2[] = "bt1-Instructions";
int difficulty = 1;
char Start3[] = "bt2-Start";

char gameOver[] = "bt1 to continue";
char savescore[] = "Enter name:";
char nameinst[] = "bt 1- change letter";
char nameinst1[] = "bt 2- save letter";
int mainload = 1;
char inst1[] = "bt 1 - Go up";
char inst2[] = "bt 2 - Go Down";
char inst3[] = "bt 3 - Fire!";
char inst4[] = "boxes have ammo";
char inst5[] = "<-1,(exit),3->";


int gameovercheck = 0;
char buffer[5]; 
char Name[3];
char Score[12][15];


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
	
	PR2 = 1953;
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
        if(b[i].x>=0 && b[i].x<105){
            for(i2=0;i2<8;i2++)
            { 
                Screen[16+b[i].x+i2+128*b[i].y] = bullet[i2];//bullet image;
            }
        }
    }
    int t1;
    for(t1=0;t1<14;t1++)
    {   
        if(ast[t1].x>=0 && ast[t1].x<105){
            for(t=0; t< 8; t++){
                Screen[16+ast[t1].x+t+(128*ast[t1].y)] = astroid[t];//ast image;
            }
        }
    }
    for(i3 = 0; i3 < 16; i3++){
        Screen[i3 + (128 * Alpha.y)] = Ship[i3];//image ship
    }
    if(box.x>0 && box.x<105){
        for(j1=0;j1<8;j1++)
        {
           Screen[16+box.x+j1+(box.y*128)] = Supply[j1];
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
                ammo = ammo + 5;    //ammo box, increases ammo by 5
                box.x = 224;
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
            
                ast[i].x = ast[i].x + 104;
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
      if(b[j].x >= 0 && b[j].x<105)
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
            ast[i].x = 112;
            ast[i].y = rand() % 4;
        }
        ast[i].x--;
    }
    if(box.x<=0)
    {
        box.x = 224;
        box.y = rand() % 4;
    }
    box.x--;
    bulletcol();
}

void Spawn()
{
    int i;
    difficulty = 1;
    counter = 0;
    diffauto = 0;
    ammo = 10;
    health = 3;
    int k = 112;
    for(i = 0; i<14; i++)
    {
        ast[i].x = k;
        k = k+8;
        ast[i].y = (rand() % 4); 
    }
    Alpha.x = 0;
    Alpha.y = 0;
    box.x = 224;
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
               health--;
               return 1;
        }
        while( (box.x+8 > ast[i].x) && (ast[i].x > box.x-8) && (ast[i].y == box.y))
        {
               box.y = (rand() % 4);
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
			if(Alpha.y > 0){ // move ship up 1 pos if pushed
				moveup();
				delay( 150 );
			}
			
			break;
		case 0x02  :
			if(Alpha.y < 3){ // move ship down 1 pos if pushed
				movedown();
				delay( 150 );
			}
			break;
		case 0x01  :
                        if (ammo > 0)
                        {
                                fire();
        		        delay (500);
                        }
                        break;
	}	
}


void left()
{ 
 anim1 = 0;
 if(anim >= 64)
 {
  test++;
 }
 anim++;
}

/* Interrupt Service Routine */
void user_isr(void) 
{
	if((IFS(0) & 0x0100))
    {
		IFS(0) = IFS(0) & 0xfffffeff;
		timecount++;
		spawnflag++; 
                while (mainload)
                { 
                    int i;
                    test = 0;
                    anim = 0;
                    for(i =0; i<128; i++)
                    {
             
                       screen_refresh();
                       int j;
                       for(j = test; j < anim; j++)
                       {
                           Screen[j] = logo[anim1];
                           Screen[j+256] = name[anim1];
                           Screen[j+128] = credit[anim1];
                           anim1++;
                       }
                       left();
                       display_image(0,Screen);
                       delay(60);
                   }

                   mainload=0;  
                }                  
		
		while (gameovercheck == 1 && health == 0)
                {                               // function for when ship has collided, gameover
			T2CONCLR = 0x8000;
			screen_refresh();
                        letterpos = 0;
                        sprintf(buffer,"%d",count1);
                        display_image(0,Screen);
                        display_string( 0, gameOver);
 			display_string( 1, clear);
			display_string( 2, clear);
			display_string( 3, clear);
			display_update();	
			name1 = 64;
                        Name[0]=clear[0];
                        Name[1]=clear[0];
                        Name[2]=clear[0];
                        while(letterpos < 3)
                        {
                            if(getbtns() == 0x04)
                            {                       // press button to move through the letters
			        name1++;
                                if(name1>90)
                                {
                                  name1 = 65;
                                }
                        	char c = name1;
                        	Name[letterpos] = c;
                        	display_string( 0, savescore );
                        	display_string( 1, nameinst );
                        	display_string( 2, nameinst1 );
                        	display_string( 3, Name );
                                display_update();
                                
                                delay(150);
                            }
                            if(getbtns() == 0x02)
                            {                      
                                letterpos++;
                                
                          	name1 = 65;
                                char c = name1;
                        	
                                if(letterpos<3)
                                {
                                	Name[letterpos] = c;
                                        display_string( 0, savescore );
                        		display_string( 1, nameinst );
                        		display_string( 2, nameinst1 );
                        		display_string( 3, Name );
                                	display_update();
                                }
                                delay(150);
                                
			    }
                        }
                        int j = 0;
                        int temp = 0;
                        for(j = 0; j<12;j++)         //determining the position of new high score
                        { 
                               if(count1>Highscore[j])
                               {
                               		temp = j;
 					break;
			       }
                        }
                        int v;
                        for(j = 11; j > temp;j--)
                        {
                             Highscore[j]=Highscore[j-1];
                             for(v =0;v<15;v++)
                             {
                             	Score[j][v] = Score[j-1][v];
                             }
                        }
                        Highscore[temp] = count1;

                        
                        for(v = 0; v<3;v++)
                        {
                            Score[temp][v] = Name[v]; 
                        }
                        Score[temp][3] = clear[0];
                        Score[temp][4] = clear[0];
			Score[temp][5] = clear[0];
                        for(v = 6; v< 11;v++)
                        {
                            int s = v-6;
                            Score[temp][v] = buffer[s]; 
                        }

                        spawnflag = 0;
			start = 1;
			gameovercheck = 0;
			delay(300);
			T2CONSET = 0x8000;
		         
                }


		
        while(start) 
        {                         // function for start
            count1= 0;
            T2CONCLR = 0x8000;
            screen_refresh();
                     
            display_string( 0, Start1 );
            display_string( 1, Start2 );
            display_string( 2, Start3);
            display_string( 3, Start4 );
            display_update();
            	
            gameovercheck = 0;

            if(getbtns() == 0x04)
            {   
                display_string( 0, inst1 );
                display_string( 1, inst2 );
                display_string( 2, inst3);
                display_string( 3, inst4 );
                display_update();
                delay(5000);
            }
            int q = 0;
            if(getbtns() == 0x01)
            {
              q=1;
              delay(500);   
            }
            int f = 0;
            while(q==1)
            { 
                if(getbtns() == 0x01)
                {
                  if(f<9)
                  {
                    f = f+3;
                    delay(150);
                  }
                }
                if(getbtns() == 0x04)
                {
                  if(f>0)
                  {
                    f = f-3;
                    delay(150);
                  }
                }
                display_string( 0, Score[0+f] );
                display_string( 1, Score[1+f] );
                display_string( 2, Score[2+f]);
                display_string( 3, inst5 );
                display_update();
                  
                      
                if(getbtns() == 0x02)
            		{
                	     q=0;
                             delay(200);	
                             break;
                        }
                
                     
            }
		
            if(getbtns() == 0x02)
            {
                     screen_refresh();
                     Spawn();
                     ScreenUpdate();
                     display_image(0,Screen);
                     start = 0;
                     T2CONSET = 0x8000;
                     delay(100);
                     break;
            }
        
	}
         
        if(diffauto == 640)
        {
           if(difficulty<12)
            {
            difficulty++;
            diffauto = 0;
            }
        }
            
        if(spawnflag == 12/difficulty)
        {                                  // determines how fast objects move and spawn
            moveright();
            moveleft();
                               
            spawnflag = 0;
            ScreenUpdate();
            display_image(0,Screen);
            gameovercheck = Astroidcollison();             
            diffauto++;
            ScoreUp();
           
            
            
        }
    	
     }	
}
