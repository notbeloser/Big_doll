#include "stdio.h"
#include "string.h"
#include "math.h"
//const int J_BT[5]={PE3,PE4,PE5,PE6,PE7};
//const int mouth_sw=PE2;
//const int K1[8]={PJ4,PJ5,PJ6,PJ7,PA7,PA6,PA5,PA4};
//const int LED1[4]={PA3,PA2,PA1,PA0};
//
//const int K2[8]={PC0,PC1,PC2,PC3,PC4,PC5,PC6,PC7};
//const int LED2[4]={PJ0,PJ1,PJ2,PJ3};
//
//const int K3[8]={PB4,PB5,PB6,PB7,PL0,PL1,PL2,PL3};
//const int LED3[4]={PL4,PL5,PL6,PL7};
//
//const int K4[8]={PD0,PD1,PD2,PD3,PD4,PD5,PD6,PD7};
//const int LED4[4]={PG5,PG4,PG3,PG2};
//const int JD[5][2]={A2,A3,
//									A4,A5,
//									A6,A7,
//									A8,A9,
//									A10,A11};
//const int svr[5]={A0,A1,A12,A13,A14};
int led[4];
int a;
long int t,kb_t;
char c[200];
char s[200];
int c_index=0;
int l1,l2,l3,l4;
void key_scan(int l[4]){

  static int i;
  static unsigned int num[4];

  num[0]=num[0]|(~(PINA>>4)&15)<<(4*i);
  num[2]=num[2]|((~PINL)&15)<<(4*i);
  
  if(i<3){
    DDRJ= _BV(6-i)|15;//keyboard 1
    PORTJ=((~_BV(6-i))&0xF0)|(PORTJ& 0xF);
    
    DDRB= _BV(i+5);//keyboard 3
    PORTB=(~_BV(i+5))&0xF0;
    
    PORTA=(PORTA&0xF0)|((l[0]>>((i+1)*4))&0xF);//LED 0
    PORTJ= (PORTJ&0xF0) | (l[1]>>((i+1)*4))&0xF;//LED 1
    PORTL= PORTL&0xF | (((l[2]>>((i+1)*4))&0xF)<<4);//LED 2
    PORTG = (PORTG & B11100001) | (((l[3]>>((i+1)*4))&0xF)<<1);//LED 3
    i++;
  }
  else{
    DDRJ= _BV(7)|15;//keyboard 1
    PORTJ=((~_BV(7))&0xF0)|(PORTJ& 0xF);
    
    DDRB= _BV(4);//keyboard 3
    PORTB=(~_BV(4))&0xF0;
    
    PORTA=(PORTA&0xF0)|(l[0]&0xF);//LED 0
    PORTJ= (PORTJ&0xF0) | l[1]&0xF;//LED 1
    PORTL= PORTL&0xF | ((l[2]&0xF)<<4);//LED 2
    PORTG = (PORTG & B11100001) | ((l[3]&0xF)<<1);//LED 3
    i=0;
    sprintf(s,"keyboard %X,%X,%X,%X ",num[0],num[1],num[2],num[3]);
    memset(num,0,sizeof(num));
  }
  
  
}


void setup() {
	PORTE|=	B11111100; //J_BT INPUT_PULLUP

	DDRJ |= B11110000; // K1 IO setting with led  
  DDRA |= B00001111;
  PORTA = B11110000;
  
  DDRC  = B00001111;// K2 IO setting with led
  DDRJ |= B00001111; 
  PORTC = B11110000;

  DDRB|=  B11110000;// K3 
  PORTL=  B00001111;
  DDRL=   B11110000;

  DDRD=   B00001111;// K4
  PORTD=  B11110000;
  DDRG |= B00011110;

  TIMSK1 = _BV(TOIE1);
	Serial.begin(115200);
}
void loop() {
  
  while(Serial.available()){
    c[c_index]=Serial.read();
    c_index++;
    delayMicroseconds(500);
  }
  if(c_index){
    sscanf(c,"%x,%x,%x,%x\n",&l1,&l2,&l3,&l4);
    sprintf(c,"");
    led[0]=l1;
    led[1]=l2;
    led[2]=l3;
    led[3]=l4;
    c_index=0;
  }

  if(millis()-t>50){
    sprintf(c,"LED %x %x %x %x ",led[0],led[1],led[2],led[3]);
    Serial.print(c);
    Serial.print(s);//keyboard
    sprintf(c,"svr %d,%d,%d,%d,%d ",analogRead(A0),analogRead(A1),analogRead(A12),analogRead(A13),analogRead(A14));
    Serial.print(c);
    sprintf(c,"JD1 %d,%d JD2 %d,%d JD3 %d,%d JD4 %d,%d JD5 %d,%d ",
      analogRead(A2),analogRead(A3),analogRead(A4),analogRead(A5),analogRead(A6),analogRead(A7),analogRead(A8),analogRead(A9),analogRead(A10),analogRead(A11));  
    Serial.print(c);
    sprintf(c,"BT %x\n",(~(PINE>>2))&B111111);
    Serial.print(c);
    t=millis();
  }
}
ISR(TIMER1_OVF_vect){
  key_scan(led);
}

