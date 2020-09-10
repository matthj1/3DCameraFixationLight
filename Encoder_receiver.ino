#include <Wire.h>
#include <Adafruit_CircuitPlayground.h>

//--------------- Settings variables

int mysettings[] = {0, 0, 0, 0, 0};

int programnumber = 0;

int brightnessvalues[] = {1,2,5,10,20,30,50,100,150,200,255};

int speedsetting[] = {500,400,300,200,100,75,50,25,15,10,5};

//--------------- Shared variables

uint32_t colorcodes [] = {
0xff0000,
0xff6f00,
0xf6ff00,
0x09ff00,
0x00fff2,
0x000dff,
0x4c00ff,
0xff008c,
0x000000  
};

int backcolor = 0;
int pixelnumon = 0;

const int speaker = 5;

//---------------- Program 1 variables


long oninterval = 50;
long nexton = 0;

int sloworfast = 0;

//----------------- Program 2 variables

long flashprev = 0;
long flashinterval = 100;
boolean ledstate = 0;

//---------------- Program 3 variables

long spinprevtime = 0;
int pixelnumoff = 0;

//---------------- Program 4 variables

long pongprevtime = 0;
bool dir = 0;

//-------------- Program 5 variables

int pixelnumon2 = 9;
long doublepongprevtime = 0;
int doublepongcolor1 = 0;
int doublepongcolor2 = 3;

//-------------- Program 6 variables

long chaseprevtime1 = 0;
long chaseprevtime2 = 0;
int chasecolor1 = 0;
int chasecolor2 = 1;

//----------------- Bleep variables

float noteperiod []
{956, 851, 758, 716, 638, 568, 506, 478, 426, 379};
int notenumber = 0;
long notelength = 15000;
bool noteon = false;
bool squarewave = LOW;

// ---------------- End of variables

//----------------- Bleep class

class bleep{
  public:
  long curtimebleeptrigger;
  long curtimebleepplaynote;
  long prevtimebleepplaynote;
  long noteoff;
  long prevfreq;

    void bleeptrigger(long i){
      curtimebleeptrigger = i;
      noteoff = curtimebleeptrigger + notelength;
      noteon = true;
    }

    void playnote(){
      curtimebleepplaynote = micros();
      if(noteon == true){
        if(curtimebleepplaynote < noteoff){
          if(curtimebleepplaynote > prevtimebleepplaynote + noteperiod[7]){
            squarewave = !squarewave;
           digitalWrite(speaker, squarewave);
            prevtimebleepplaynote = curtimebleepplaynote;           
         }          
        }
        else{
          noteon = false;
           Serial.println("Stopped");
        }
      }
    }


};

bleep mybleep;

void setup() {
  CircuitPlayground.begin();
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  pinMode(speaker, OUTPUT);
}

void loop() {
  mybleep.playnote();
  requestsettings();
  
  switch (mysettings[0]){
    case 0 : off();
      break;
    case 1 : rainbowloop();
      break;
    case 2 : flash();
      break;
    case 3 : spin();
      break;
    case 4 : pong();
      break; 
    case 5 : doublepong();
      break;  
    case 6 : chase();
      break;
    case 9 : editmode();
      break;
  }
}

//---------------------------- Get settings

int requestsettings(){
  Wire.requestFrom(8, 5);

  while (Wire.available() > 0) {
    for (int x = 0; x < 5; x++){
      mysettings[x] = Wire.read();
      //Serial.print(mysettings[x]);
      //Serial.print(" ");
    }
   // Serial.println("");
  }
}

//-------------------------- Off Program 0

void off(){
for (pixelnumon = 0; pixelnumon < 10; pixelnumon++)
      {
       CircuitPlayground.setPixelColor(pixelnumon, colorcodes[8]);
      } 
}


//------------------------------------------- Program 1


void rainbowloop (){
  
CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

unsigned long currenttime = millis();

if(currenttime > nexton + speedsetting[mysettings[2]] ){
  if (pixelnumon < 10){
    CircuitPlayground.setPixelColor(pixelnumon, colorcodes[backcolor]);
    Serial.println ("LED on");
    pixelnumon++;
    nexton = currenttime;
  }
   else{
  pixelnumon = 0;
  Serial.println ("I'm at the end");
  if (backcolor < 7){
    backcolor++;
  }
  else{
    backcolor = 0;
  }
 }
}
}

//---------------------------------------------- Program 2

void flash (){

CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

  unsigned long  currenttime = millis();
  
  if (currenttime > flashprev + speedsetting[mysettings[2]])
  {
    if (ledstate == 0)
    {
      ledstate = !ledstate;
      mybleep.bleeptrigger(micros());
       for (pixelnumon = 0; pixelnumon < 10; pixelnumon++)
       {
        CircuitPlayground.setPixelColor(pixelnumon, colorcodes[backcolor]);
       }
         flashprev = currenttime;
     }
     else
    {
    ledstate = !ledstate;
    for (pixelnumon = 0; pixelnumon < 10; pixelnumon++)
      {
       CircuitPlayground.setPixelColor(pixelnumon, colorcodes[8]);
      }
      flashprev = currenttime;
     }
     if (backcolor < 7)
     {
      backcolor++; 
     }
     else {
      backcolor = 0;
     }
  }
}

//--------------- Program 3

void spin(){

CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

unsigned long  currenttime = millis();

if(currenttime > spinprevtime + speedsetting[mysettings[2]] ){
  if (pixelnumon < 10){
    CircuitPlayground.setPixelColor(pixelnumon, colorcodes[backcolor]);
    Serial.print ("I turned on pixel number ");
    Serial.println (pixelnumon);
    
   for (pixelnumoff = 0; pixelnumoff < 10; pixelnumoff++){
      if(pixelnumon != pixelnumoff){
         CircuitPlayground.setPixelColor(pixelnumoff, colorcodes[8]);
         Serial.print ("I turned off pixel number ");
         Serial.println(pixelnumoff);       
      }
   }
    pixelnumon++;
    spinprevtime = currenttime;
  }
   else{
  pixelnumon = 0;
  mybleep.bleeptrigger(micros());
  if (backcolor < 7){
    backcolor++;
  }
  else{
    backcolor = 0;
  }
 }
}
}

//--------------------- Program 4

void pong(){

CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

unsigned long  currenttime = millis();

if(currenttime > pongprevtime + speedsetting[mysettings[2]] ){
  if(dir == 0){
    if(pixelnumon < 10){
     CircuitPlayground.setPixelColor(pixelnumon, colorcodes[backcolor]);
    Serial.print ("I turned on pixel number ");
    Serial.println (pixelnumon);
    
   for (pixelnumoff = 0; pixelnumoff < 10; pixelnumoff++){
      if(pixelnumon != pixelnumoff){
         CircuitPlayground.setPixelColor(pixelnumoff, colorcodes[8]);
         Serial.print ("I turned off pixel number ");
         Serial.println(pixelnumoff);       
      }
   }
    pixelnumon++;
    pongprevtime = currenttime;
    }
    else{
      mybleep.bleeptrigger(micros());
      dir = 1;
      pixelnumon = pixelnumon -2;
            if(backcolor < 7){
      backcolor++;        
      }
      else{
        backcolor = 0;
      }
    }
  }
   else{
    if(pixelnumon > 0){
      CircuitPlayground.setPixelColor(pixelnumon, colorcodes[backcolor]);
          Serial.print ("I turned on pixel number ");
          Serial.println (pixelnumon);
          
         for (pixelnumoff = 0; pixelnumoff < 10; pixelnumoff++){
            if(pixelnumon != pixelnumoff){
               CircuitPlayground.setPixelColor(pixelnumoff, colorcodes[8]);
               Serial.print ("I turned off pixel number ");
               Serial.println(pixelnumoff);       
            }
         }
          pixelnumon--;
          pongprevtime = currenttime;
          }
    else{
      mybleep.bleeptrigger(micros());
      dir = 0;
      if(backcolor < 7){
      backcolor++;        
      }
      else{
        backcolor = 0;
      }
    }
   }
}
}

//--------------------------Program 5

void doublepong(){

CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

unsigned long  currenttime = millis();

if(currenttime > doublepongprevtime + speedsetting[mysettings[2]] ){
    if(pixelnumon < 10){
     CircuitPlayground.setPixelColor(pixelnumon, colorcodes[doublepongcolor1]);
     CircuitPlayground.setPixelColor(pixelnumon2, colorcodes[doublepongcolor2]);
   for (pixelnumoff = 0; pixelnumoff < 10; pixelnumoff++){
      if((pixelnumon != pixelnumoff) && (pixelnumon2 != pixelnumoff) ){
         CircuitPlayground.setPixelColor(pixelnumoff, colorcodes[8]);
      }
   }
    pixelnumon++;
    pixelnumon2--;
    doublepongprevtime = currenttime;
    }
    else{
      mybleep.bleeptrigger(micros());
      pixelnumon = 0;
      pixelnumon2 = 9;
      if (doublepongcolor1 < 7){
        doublepongcolor1++;
      }
      else{
        doublepongcolor1 = 0;
      }
      if(doublepongcolor2 < 7){
        doublepongcolor2++;
      }
      else{
        doublepongcolor2 = 0;
      }
    }
  
}
}

//------------------------- Program 6

void chase(){
  //interval pixels on at two different speeds
  //turn off pixels that aren't meant to be on
  //when two pixel numbers are equal, change colour of pixels

CircuitPlayground.setBrightness(brightnessvalues[mysettings[1]]);

unsigned long  currenttime = millis();

  if(currenttime > chaseprevtime1 + speedsetting[mysettings[2]] ){
   if(pixelnumon < 10){
    CircuitPlayground.setPixelColor(pixelnumon, colorcodes[chasecolor1]);
    chaseprevtime1 = currenttime;
    pixelnumon++;
  }
  else{
    pixelnumon = 0;
  }
}
  if(currenttime > chaseprevtime2 + (speedsetting[mysettings[2]]/1.5 )){
     if(pixelnumon2 < 10){
      CircuitPlayground.setPixelColor(pixelnumon2, colorcodes[chasecolor2]);
      chaseprevtime2 = currenttime;
      pixelnumon2++;
    }
    else{
      pixelnumon2 = 0;
    }
    if(pixelnumon2 == pixelnumon){
      mybleep.bleeptrigger(micros());
      if(chasecolor2 < 7){
         chasecolor2++;
        }
      else{
         chasecolor2 =0;
        }
      if(chasecolor1 < 7){
         chasecolor1++;
        }
      else{
         chasecolor1 =0;
        }  
    }
  }
}

//---------------------------------- Edit mode

void editmode(){

  CircuitPlayground.setBrightness(brightnessvalues[3]);
  
  if (mysettings[3] == 1){
    for (pixelnumon = 0; pixelnumon < mysettings[1]; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[0]);
          }
        for (pixelnumon = mysettings[1]; pixelnumon < 10; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[8]);
          }
  }
  else if (mysettings[3] == 2){
    for (pixelnumon = 0; pixelnumon < mysettings[2]; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[4]);
          }
        for (pixelnumon = mysettings[2]; pixelnumon < 10; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[8]);
          }
  }
   else if (mysettings[3] == 3){
    if (mysettings[4] % 2 == 0){
        for (pixelnumon = 0; pixelnumon < 10; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[0]);
          }    
    }
    else {
      for (pixelnumon = 0; pixelnumon < 10; pixelnumon++)
          {
           CircuitPlayground.setPixelColor(pixelnumon, colorcodes[4]);
          } 
    }
  }
}
