


#include "micro+pulsesensor.h"
int motors3= D2;
int motors5= D3;


             

int ledState = LOW;             




void setup(){
  Serial.begin(115200);
  start("Spider","spidertronix");

  pinMode(motors3,OUTPUT);
  pinMode(motors5,OUTPUT);
  pinMode(led_pin,OUTPUT);        
          
}

void loop(){

  //digitalWrite(pLED, LOW);
  waitUntilNewReq(); 

  if (getPath()=="/3_MOTORS_ON"){
    digitalWrite(motors3,HIGH);
    digitalWrite(motors5,HIGH);
  }
  if (getPath()=="/5_MOTORS_ON"){
    digitalWrite(motors5,HIGH);
    digitalWrite(motors3,LOW);
  }  //if (getPath()=="/3_LED"){
    //analogWrite(LED,255);
  //}
  //if (getPath()=="/PWM_LED"){
    //analogWrite(LED,10);
  //}
  if (getPath()=="/CLOSE_MOTORS"){
    digitalWrite(motors3,LOW);
    digitalWrite(motors5,LOW);
    
  }
  
}
