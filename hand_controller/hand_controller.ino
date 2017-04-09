#include "string.h"
const int sw[4]={6,7,8,9};
const int joy_sw=10;
const int joy_x=14;
const int joy_y=15;
int sw_state; 
int joy_x_value,joy_y_value;
char s[100];
void setup() {
  for(int i=0;i<4;i++){
    pinMode(sw[i],INPUT_PULLUP);
  }
  pinMode(joy_sw,INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  sw_state= (!digitalRead(joy_sw)<<4)|(!digitalRead(sw[3])<<3)|(!digitalRead(sw[2])<<2)|(!digitalRead(sw[1])<<1)|(!digitalRead(sw[0]));
  joy_x_value=analogRead(joy_x);
  joy_y_value=analogRead(joy_y);
  sprintf(s,"%d,%d,%d\n",sw_state,joy_x_value,joy_y_value);
  Serial.print(s);
  memset(s,0,sizeof(s));
  delay(50);
}
