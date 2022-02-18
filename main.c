////////////////////////////////

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config OSCS = OFF      // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is enabled (oscillator switching is enabled))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset enable

#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

///////////////////////////////
#include<p18f452.h>
#include <stdio.h>  
#include <stdlib.h>
#pragma config WDT=OFF
#define r1 PORTCbits.RC0
#define r2 PORTCbits.RC1
#define r3 PORTCbits.RC2
#define r4 PORTCbits.RC3
#define r5 PORTCbits.RC4
#define r6 PORTCbits.RC5
#define r7 PORTCbits.RC6
#define r8 PORTCbits.RC7
#define c1 PORTDbits.RD0
#define c2 PORTDbits.RD1
#define c3 PORTDbits.RD2
#define c4 PORTDbits.RD3
#define c5 PORTDbits.RD4
#define c6 PORTDbits.RD5
#define c7 PORTDbits.RD6
#define c8 PORTDbits.RD7
#define e PORTEbits.RE0
#define c PORTBbits.RB7
#define g PORTBbits.RB6
#define _XTAL_FREQ 10000000

int i=0,j=0;
unsigned char filled=0,r,pos; //filled will count the number of column we want to show turned on all the time and pos is current position
void delay(void);
void move_left(void);
void move_right(void);
void led(unsigned char r);
void chk_isr(void);
#pragma interrupt chk_isr
void chk_isr(void)       //high priority interupt 
{
  if(INTCONbits.INT0IF == 1)            
   move_right();   //if intrupt is at  RB0
  else if(INTCON3bits.INT1IF == 1)   //interupt at RB1
   {move_left();}
}

void reset_col(void){   //this function will turn on the column according to current score of which is called filled....
delay();
if(filled==0)
	PORTD=0x00;
else if(filled==1)
	PORTD=0x01; //hex format of 00000001 c1=1
else if(filled==2)
	PORTD=0x03;
else if(filled==3)    
	PORTD=0x07;
else if(filled==4)
	PORTD=0x0f;
else if(filled==5)
	PORTD=0x1f;
else if(filled==6)
	PORTD=0x3f;
else if(filled==7)
	PORTD=0x7f;
else if(filled==8)
	PORTD=0xff;
}
void reset_row(void){PORTC=0xff;}   //turn off all the rows 
void generate_r(void);  //will generate a random number this is not a complete random  as it will follow a similar pattern 
void col_on(unsigned char col){   //this function will trun on and blink the the col according to our current  position 
PORTC=0;
reset_col();
if(col==1){delay();c1=1;delay();}
else if(col==2){delay();c2=1;delay();}
else if(col==3){delay();c3=1;delay();}
else if(col==4){delay();c4=1;delay();}
else if(col==5){delay();c5=1;delay();}
else if(col==6){delay();c6=1;delay();}
else if(col==7){delay();c7=1;delay();}
else if(col==8){delay();c8=1;delay();}}

void turn_on(unsigned char col){     //it will trun on whole column by setting all the rows equal to 0 
PORTC=0;
}
void main(void){
TRISC=0;
TRISD=0;
TRISB=0xff;
TRISA=0x00;
TRISE=0;
TRISBbits.RB7=0;
TRISBbits.RB6=0;
INTCONbits.INT0IF=0;    //enabling intrupts and  reseting flags 
INTCONbits.INT0IE=1;
INTCON3bits.INT1IF = 0;  
INTCON3bits.INT1IE = 1; 
INTCONbits.GIE=1;
filled=0;
pos=1;
generate_r();
while(1){
col_on(pos);
}
}
void move_right(void){
pos=pos+1;
if(pos>8){pos=1+filled;}
if (pos==r) {filled++; generate_r();}
if(filled==8){
led(0);
}
delay();
INTCONbits.INT0IF=0;
}
void move_left(void){
pos=pos-1;
if(pos<1+filled){pos=8;}
if (pos==r) {filled++; generate_r();}
if(filled==8){
led(0);}
delay();
INTCON3bits.INT1IF=0;

}
void generate_r(void){
reset_col();
r=(rand()%(8-filled))+1+filled;
if(r==pos&&filled!=7){
while(r==pos)r=(rand()%(8-filled))+1+filled;}
led(r);
}
void led(unsigned char r){
if(r==1){PORTA=0x06;e=0;}
else if(r==2){PORTA=0x5b;e=1;g=1;c=1;}
else if(r==3){PORTA=0x4f;e=0;g=1;c=1;}
else if(r==4){PORTA=0x66;e=0;g=1;c=1;}
else if(r==5){PORTA=0x6d;e=0;g=1;c=1;}
else if(r==6){PORTA=0x7d;e=1;g=1;c=1;}
else if(r==7){PORTA=0x07;e=0;g=0;c=1;}
else if(r==8){PORTA=0xff;e=1;g=1;c=1;}
else if(r==0){c=0;PORTA=0x3f;e=1;g=0;TRISC=0xff;}
}
void delay(void){
for(i=0;i<1000;i++){
for(j=0;j<165;j++);
}}