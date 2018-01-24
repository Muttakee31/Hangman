/*
 * Hangman.c
 *
 * Created: 1/8/2018 12:07:10 PM
 *  Author: hp
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "pingsr4.h"
//header to enable data flow control over pins
#define F_CPU 20000000
//telling controller crystal frequency attached
//header to enable delay function in program
static int r8,wr,xxxx=0;
static int wordlist[10][5]={{16,5,1,3,5},{13,1,7,9,3},{6,12,1,19,8},{3,1,3,8,5},{19,20,5,1,11},{18,8,25,13,5},{19,14,15,23,25},{10,15,11,5,18},{15,6,20,5,14},{12,15,7,1,14}};
static int flag[7] = {0,0,0,0,0,0,0};
static int t1;
static int flagtemp[5][7]={
	{0,19,20,1,18,20,0},{0,25,15,21,23,9,14},{0,8,1,14,7,5,4},{0,8,1,14,7,5,4},{0,8,1,14,7,5,4}};		
	static int ALPHAnew[27][8]={
			{0,0,0,0,0,0,0b11110000,0},
			{0,0b01100000,0b10010000,0b11110000,0b10010000,0b10010000,0,0}, //a
			{0,0b11100000,0b10010000,0b11100000,0b10010000,0b11100000,0,0}, //b
			{0,0b11110000,0b10000000,0b10000000,0b10000000,0b11110000,0,0}, //c
			{0,0b11100000,0b10010000,0b10010000,0b10010000,0b11100000,0,0}, //d
			{0,0b11110000,0b10000000,0b11110000,0b10000000,0b11110000,0,0}, //e
			{0,0b11110000,0b10000000,0b11100000,0b10000000,0b10000000,0,0}, //f
			{0,0b11110000,0b10000000,0b10110000,0b10010000,0b11110000,0,0}, //g
			{0,0b10010000,0b10010000,0b11110000,0b10010000,0b10010000,0,0}, //h
			{0,0b11100000,0b01000000,0b01000000,0b01000000,0b11100000,0,0}, //i
			{0,0b11110000,0b00100000,0b00100000,0b10100000,0b01100000,0,0}, //j
			{0,0b10010000,0b10100000,0b11000000,0b10100000,0b10010000,0,0}, //k
			{0,0b10000000,0b10000000,0b10000000,0b10000000,0b11110000,0,0},  //l
			{0,0b10010000,0b11110000,0b10010000,0b10010000,0b10010000,0,0}, //m
			{0,0b10010000,0b11010000,0b10110000,0b10010000,0,0,0}, //n
			{0,0b01100000,0b10010000,0b10010000,0b10010000,0b01100000,0,0}, //o
			{0,0b11100000,0b10010000,0b10100000,0b10000000,0b10000000,0,0},  //p
			{0,0b01100000,0b10010000,0b11010000,0b10110000,0b00110000,0,0}, //q
			{0,0b11100000,0b10010000,0b11100000,0b10100000,0b10010000,0,0}, //r
			{0,0b11110000,0b10000000,0b11110000,0b00010000,0b11110000,0,0}, //s
			{0,0b11110000,0b01100000,0b01100000,0b01100000,0b01100000,0,0}, //t
			{0,0b10010000,0b10010000,0b10010000,0b10010000,0b11110000,0,0}, //u
			{0,0b10010000,0b10010000,0b10010000,0b10010000,0b01100000,0,0}, //v
			{0,0b10010000,0b10010000,0b10010000,0b11110000,0b10010000,0,0}, //w
			{0,0b10010000,0b01100000,0b01100000,0b10010000,0b00000000,0,0}, //x
			{0,0b10010000,0b10010000,0b10010000,0b01100000,0b01100000,0,0}, //y
			{0,0b11110000,0b00100000,0b01000000,0b11110000,0,0,0}, //z
			{0,0b00110000,0b10010000,0b11110000,0b10010000,0b10010000,0,0}, //a again
		};//characters a,		
	static int hangman[8]={0b00000100,0b00000110,0b00000110,0b000011111,0b00000100,0b00001010,0b0010001,0b00000000};
	static int hangman4[8][8]={
		    {0b01111100,0b00010000,0b01000100,0b00111000,0b00010000,0b00010000,0b00101000,0b01000100},
			{0b01111100,0b00010000,0,0,0,0,0,0},
			{0b01111100,0b00010000,0b00000000,0b00010000,0b00010000,0b00000000,0b00000000,0},
			{0b01111100,0b00010000,0b00000000,0b00010000,0b00010000,0b00100000,0b01000000,0},
			{0b01111100,0b00010000,0b00000000,0b00010000,0b00010000,0b00101000,0b01000100,0},
			{0b01111100,0b00010000,0b00000000,0b011100,0b00010000,0b00101000,0b01000100,0},
			{0b01111100,0b00010000,0b00000000,0b01111100,0b00010000,0b00101000,0b01000100,0},
			{0b01111100,0b00010000,0b00010000,0b01111100,0b00010000,0b00101000,0b01000100,0}
			};
	uint8_t l =0;
	int lol;
int main(void)
{
	//int xx;
	//while(1){
	lol=0;
	int inc;
	DDRD = 0xFF;//PORTB,C,D are set as output
	DDRA = 0xFF;
	DDRC = 0xFF;	
	uint16_t r;
	US_init();
	int distance;
	int distance2;		
	DDRA = 0xff;
	DDRC = 0xff;
	
	while(1){
		
		t1=1;
		int xx=0;
			
		while(1)
		{			
			US_trig();
			r=pulse_width();
			distance=(r/58);
			distance2=(r/148);
			
			if(distance >0.1){			
				if(xx==0){int_flag(2);}
				else{
					xx=0;
					int_flag(0);
					break;
				}
			}
			else{
				if(xx==0){
					xx=1;
				}
			}
			checkdemux(xx);
		}		
		r8=0;
		wr=0;	
		while(1)
		{		
			int xx;
			xx=0;	
			US_trig();
			r=pulse_width();
			distance=(r/58);
			distance2=(r/148);				
			if(distance >0.1){
				PORTC= PORTC|0b01000000;
				if(flag[1]!=0){
					for(int i=0;i<5;i++){
						if(wordlist[lol][i]==flag[1]){
							if(flag[i+2]==0){
								flag[i+2]=flag[1];
								xx=1;
								r8++;							
							}
						}
					}
					flag[1]=0;
					if(xx==0){
						t1++;
						wr++;
					}
				}
			}
			else{
				PORTC= PORTC&0b10111111;
				flag[1]++;
			}
			checkdemux(xx);
			if(r8==5||wr==6){
				break;
			}
		}	
		if(r8==5){
			int_flag(3);
			t1=0;
			for(inc=0;inc<9;inc++){
			checkdemux(xx);}
		}
		else if(wr==6){
			t1=6;
			int_flag(1);
			for(inc=0;inc<6;inc++){
				checkdemux(xx);
			}
			int_flag(4);
			for(inc=0;inc<6;inc++){
				checkdemux(xx);
			}
		
		}
		lol++;
		
	}
	
	
}
void int_flag(int xxx){
	int i;
	for(i=0;i<7;i++){
		if(xxx==0){flag[i]=0;}
		else if(xxx==1){
			if(i<2)flag[i]=0;
			else flag[i]=wordlist[lol][i-2];
			}
		else if(xxx==2){flag[i]=flagtemp[0][i];}
		else if(xxx==3){flag[i]=flagtemp[1][i];}
		else if(xxx==4){flag[i]=flagtemp[2][i];}	
	}	
}

void checkdemux(int xx)
{
	int i,j,k,tempAlpha,temp;
	unsigned char column;
	temp=1;
	for (int n=0;n<50;n++){ //to catch eye
		PORTC = (PORTC & 0b11111110)|0b00000010;
		for (int j=0;j<16;j++)
		{
			PORTD=j|(j<<4);
			temp=0b00001111;
			temp = flag[2];
			temp=ALPHAnew[temp][j%8];
			temp=(temp>>4)&0b00001111;
			if(j<8){PORTA = ALPHAnew[flag[1]][j%8]|(ALPHAnew[flag[2]][j%8]>>4);}
			else {PORTA = ALPHAnew[flag[3]][j%8]|(ALPHAnew[flag[4]][j%8]>>4);}
			_delay_us(600);
		}
		PORTC = (PORTC & 0b11111101)|0b00000001;
		for (int j=0;j<16;j++)
		{
			PORTD=j|(j<<4);
			if(j<8){PORTA = ALPHAnew[flag[5]][j%8]|(ALPHAnew[flag[6]][j%8]>>4);}
			else {PORTA = hangman4[t1][j%8];}
			_delay_us(500);
		}
		
	}
}