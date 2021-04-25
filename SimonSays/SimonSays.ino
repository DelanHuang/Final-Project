#include "Adafruit_NeoTrellis.h"
#include "stdlib.h"

Adafruit_NeoTrellis trellis;

int OutArr[10]; //output array, max of 10 entries 
int GameSize = 10; //rounds the game with last
int inIndex = 0;
int outIndex = 0; //output array index
byte n = 17;
int timer;        // variable to adjust difficulty
int active = 0;   // variable to determine if we are in game (0 means in game)

//define a callback for key presses
TrellisCallback blink(keyEvent evt){
  // Check is the pad pressed?
  if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    trellis.pixels.setPixelColor(evt.bit.NUM, Wheel(map(evt.bit.NUM, 0, trellis.pixels.numPixels(), 0, 255))); //on rising
    
    n = evt.bit.NUM; //set n to the value of which ever button is pushed
  } 
  
  else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
  // or is the pad released?
    trellis.pixels.setPixelColor(evt.bit.NUM, 0); //off falling
  }
    
  // Turn on/off the neopixels!
  trellis.pixels.show();
  
  return 0;
}

void setup() {
  Serial.begin(9600);
  
  if (!trellis.begin()) {
    Serial.println("Could not start trellis, check wiring?");
    while(1);
  } else {
    Serial.println("NeoPixel Trellis started");
  }

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, blink);
  }
    //do a little animation to show we're on
 int lightshow[100]; 
  for (uint16_t j = 0; j < sizeof(lightshow)/2 ; j++ ){
    lightshow[j] = random(0,trellis.pixels.numPixels());
  }
  for (uint16_t i=0; i < sizeof(lightshow)/2; i++) {
    trellis.pixels.setPixelColor(lightshow[i], Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(50);
  }
  //turn off a given order
  for (uint16_t i=0; i < 100; i++) {
    trellis.pixels.setPixelColor(lightshow[i], 0x000000);
    trellis.pixels.show();
    delay(50);
  }
}



void loop() {
  delay(20); //the trellis has a resolution of around 60hz
  if (active == 0){
    StartUp();
  }
  else{
    OutArray();
    UserInput();
  }
}


/******************************************/

void StartUp(){
    for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
        trellis.pixels.setPixelColor(i, 0x000000);
        trellis.pixels.show();
        delay(50);
    }
    delay(250);

// User can press one of the top four buttons to select difficulty
  while (active == 0) {

    int RanNum = random(0, 15); //create random number
    OutArr[outIndex] = RanNum; //send random number to output array
    if(outIndex < 9){
      outIndex++;
    }
    else{
      outIndex = 0;
    }
    /*
      Serial.print("outIndex: "); //Serial monitor check for index troubleshooting
      Serial.print(outIndex);
      Serial.println();
      */
  
    // Set top row of LEDs to green, yellow, orange, and red
    trellis.pixels.setPixelColor(0, 0x00FF00);      // set first button to green
    trellis.pixels.setPixelColor(1, 0xFFFF00);      // set second button to yellow
    trellis.pixels.setPixelColor(2, 0xFF7F00);      // set second button to orange
    trellis.pixels.setPixelColor(3, 0xFF0000);      // set second button to red 
    trellis.pixels.show();
    trellis.read();
  
   if (n == 0) {
     timer = 1000;   // easiest difficulty (green)
     for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++){
        trellis.pixels.setPixelColor(i, 0x000000); // turn off pixels before starting game
        trellis.pixels.show();
     }
     active = 1;
   }
  
   else if (n == 1) {
      timer = 750;    // medium difficulty (yellow)
      for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++){
        trellis.pixels.setPixelColor(i, 0x000000); // turn off pixels before starting game
        trellis.pixels.show();
     }
     active = 1;
   }
 
   else if (n == 2) {
      timer = 500;    // mediumer difficulty (orange)
      for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++){
        trellis.pixels.setPixelColor(i, 0x000000); // turn off pixels before starting game
        trellis.pixels.show();
        active = 1;
     }
     active = 1;
   }
 
   else if (n == 3) {
      timer = 250;    // hardest difficulty (red)
      for (uint16_t i = 0; i < trellis.pixels.numPixels(); i++){
        trellis.pixels.setPixelColor(i, 0x000000); // turn off pixels before starting game
        trellis.pixels.show();
     }
     active = 1;
   }
/*   else {
      Serial.println("Please use one of the four LEDs to select difficulty");
    }*/
    
  }
  outIndex = 0;
  n = 17;
  delay(1000);
}

void OutArray(){ // Store random number into the output array/ then light up the LEDS based off of Array values. 
  
  if(outIndex <= 9){//dont output more than 10 random numbers. 

      
//light up the LEDS from output array
    for(uint16_t i=0; i <= outIndex; i++) {
      trellis.pixels.setPixelColor(OutArr[i], Wheel(map(OutArr[i], 0, trellis.pixels.numPixels(), 0, 255))); // set LED color
      trellis.pixels.show();//turn on led
      delay(timer);//delay 1 second
      trellis.pixels.setPixelColor(OutArr[i], 0); //set LED color to 0 (off)
      trellis.pixels.show();// turn off LED
      delay(timer);  //delay 1 sec
      }
   
    for(int i = 0;i <= outIndex; i++){ // serial monitor test for out array. displays array values as they are generated. 
      Serial.print("index: ");
      Serial.print(i);
      Serial.print(", Value: ");
      Serial.print(OutArr[i]);
      Serial.println(); 
      } 
      
    outIndex++; //increment index for output array
  }
}

void UserInput(){ //compare button pushes to output array
  
    while(inIndex < outIndex){ //stay in while-loop until user-inputs are inputted and completely compared to output array
      trellis.read();//keep checking for button pushes. 
      //Serial.println("*****in while-loop******");
      // add input timer for button pushes?
      
      if(n < 17){  // wait for button push, button values are from 0-15, n = 17. When a button is pushed n = which ever button value -> enters if statement.
        /*
        Serial.println("*****in 1st if statement******");
        Serial.print("you pushed: "); //view which button was pressed
        Serial.print(n);
        Serial.println();
        */
        
        trellis.pixels.setPixelColor(n, Wheel(map(n, 0, trellis.pixels.numPixels(), 0, 255))); // set LED color
        trellis.pixels.show();//turn on led
        delay(200);//delay time for LED to be turned on
        trellis.pixels.setPixelColor(n, 0); //set LED color to 0 (off)
        trellis.pixels.show();// turn off LED
        
       //compare button push value to output array
        if(n == OutArr[inIndex]){
         // Serial.println("*****in 2nd if statement******");
          Serial.println("Good Press");
         // Check if user has made it to end of game 
          if (inIndex < 9){
             inIndex++; //increment input array index 
          }
          // User has successfully completed game
          else{
          // Set all pixels to green
             Serial.println("Success!\n");  
             for(int i = 0; i < trellis.pixels.numPixels(); i++){
                trellis.pixels.setPixelColor(i, 0x00FF00);
                trellis.pixels.show();
                delay(50);
                trellis.pixels.setPixelColor(i, 0x000000);
                trellis.pixels.show();
                delay(50);
                trellis.pixels.setPixelColor(i, 0x00FF00);
                trellis.pixels.show();
                delay(50);
                trellis.pixels.setPixelColor(i, 0x000000);
                trellis.pixels.show();
                delay(50);
                trellis.pixels.setPixelColor(i, 0x00FF00);
                trellis.pixels.show();
                delay(50);
              active = 0;
              outIndex = 0;
             }
          }
          n = 17;
        }
       
       else{ 
        //display failed game animation, restart game.
        Serial.println("Bad Press"); 
        for(int i = 0; i < trellis.pixels.numPixels(); i++){
          trellis.pixels.setPixelColor(i, 0xFF0000); 
        }
        trellis.pixels.show();
        delay(1000);
        outIndex = 0;
        active = 0;
        inIndex = 10; //break while loop 
        n = 17; //reset n to any value not in 0-15, so "if" statement waits for new button push.
       }
        //Serial.println("*****After else statement******");       
      }
  }
    delay(1000);
    inIndex = 0; //reset input index to re-record input values after each round (be-able to re-enter while-loop)
}


// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  //Serial.println(WheelPos);
  if(WheelPos < 85) {
   return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
   WheelPos -= 170;
   return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}
