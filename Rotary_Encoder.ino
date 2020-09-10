#include <Wire.h>

int encoderpinA = 4;
int encoderpinB = 5;
int buttonpin = 3;

bool buttonstatecurrent = LOW;
bool buttonstateprevious = LOW;
bool dbcurrent = LOW;

int encoderposcurrent = 0;
int encoderposprevious = 0;

bool pinAstatus = LOW;
bool pinBstatus = LOW;

int counter = 0;

int mode = 0;

int mysettings[] = {0, 0, 0, 0, 0};
int mysettingsprev = 0;
int settingtoedit = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode (encoderpinA, INPUT);
  pinMode (encoderpinB, INPUT);
  pinMode (buttonpin, INPUT);
  Serial.begin (9600);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  readpushbutton();
  readencoder();
}

bool debounce (){
  
  bool dbcurrent = digitalRead(buttonpin);
  
   if (dbcurrent != buttonstateprevious){
    delay(10);
    dbcurrent = digitalRead(buttonpin);
   }
    return dbcurrent;
}


void readpushbutton(){
  
   buttonstatecurrent = debounce();
   
 if (buttonstatecurrent == HIGH && buttonstateprevious == LOW){
    if(mysettings[0] != 9){
      mysettingsprev = mysettings[0];
       mysettings[0] = 9;
       Serial.println("I'm in edit mode");     
    }
    if(mysettings[0] == 9){
      if(mysettings[3] < 3){
        mysettings[3]++;
        Serial.print("I'm editing setting number ");
        Serial.println(mysettings[3]);      
      }
      else{
        mysettings[0] = mysettingsprev;
        Serial.println("I've left edit mode");
        mysettings[3] = 0;
      }
   }
  }
  buttonstateprevious = buttonstatecurrent;
}


void readencoder(){
  
  pinAstatus = digitalRead(encoderpinA);
  pinBstatus = digitalRead(encoderpinB);

  if(pinAstatus == LOW && pinBstatus == LOW){
    encoderposcurrent = 3; 
   //Serial.println("position is 0");         
  }
  if (pinAstatus == HIGH && pinBstatus == LOW){
    encoderposcurrent = 2;
  //Serial.println("position is 1");
  }
  if (pinAstatus == HIGH && pinBstatus == HIGH){
    encoderposcurrent = 1;
// Serial.println("position is 2");
  }
  if (pinAstatus == LOW && pinBstatus == HIGH){
    encoderposcurrent = 0;
// Serial.println("position is 3");
  }

  if (encoderposcurrent != encoderposprevious){
    if ((encoderposprevious == 0 && encoderposcurrent == 1) || 
        (encoderposprevious == 1 && encoderposcurrent == 2) || 
        (encoderposprevious == 2 && encoderposcurrent == 3) || 
        (encoderposprevious == 3 && encoderposcurrent == 0))
     {
      if(mysettings[mysettings[3]] < 10){
      mysettings[mysettings[3]]++;
      Serial.println(mysettings[mysettings[3]]);
      }
    }
    else if ((encoderposprevious == 3 && encoderposcurrent == 2) || 
        (encoderposprevious == 2 && encoderposcurrent == 1) || 
        (encoderposprevious == 1 && encoderposcurrent == 0) || 
        (encoderposprevious == 0 && encoderposcurrent == 3))
     {
      if(mysettings[mysettings[3]] > 0){
      mysettings[mysettings[3]]--;
      Serial.println(mysettings[mysettings[3]]);
      }
     }
    encoderposprevious = encoderposcurrent;
  }
}

void requestEvent() {
  for(int x = 0; x < 5; x++){
     Wire.write(mysettings[x]);   
  }
}
