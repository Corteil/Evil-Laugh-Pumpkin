/*************************************************** 
 * This is an example for the Adafruit VS1053 Codec Breakout
 * 
 * Designed specifically to work with the Adafruit VS1053 Codec Breakout 
 * ----> https://www.adafruit.com/products/1381
 * 
 * Adafruit invests time and resources providing this open source code, 
 * please support Adafruit and open-source hardware by purchasing 
 * products from Adafruit!
 * 
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
 * BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <Adafruit_NeoPixel.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// freesound.org

// These can be any pins:
#define RESET 9      // VS1053 reset pin (output)
#define CS 10        // VS1053 chip select pin (output)
#define DCS 8        // VS1053 Data/command select pin (output)
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

// define pin 2

#define PIN 2

// set Arduino audio

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(RESET, CS, DCS, DREQ, CARDCS);

int randomNumber = 0;
String fileName ="";
char charBuf[50];

/* 
 Debounce
 
 Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
 press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
 a minimum delay between toggles to debounce the circuit (i.e. to ignore
 noise).  
 
 The circuit:
 * LED attached from pin 13 to ground
 * pushbutton attached from pin 2 to +5V
 * 10K resistor attached from pin 2 to ground
 
 * Note: On most Arduino boards, there is already an LED on the board
 connected to pin 13, so you don't need any extra components for this example.
 
 
 created 21 November 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Limor Fried
 modified 28 Dec 2012
 by Mike Walters
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Debounce
 */
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 18;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// setup candle

// color variables: mix RGB (0-255) for desired yellow
int redPx = 255;
int grnHigh = 100;
int bluePx = 10;

// animation time variables, with recommendations
int burnDepth = 6; //how much green dips below grnHigh for normal burn - 
int flutterDepth = 20; //maximum dip for flutter
int cycleTime = 120; //duration of one dip in milliseconds

// pay no attention to that man behind the curtain
int fDelay;
int fRep;
int flickerDepth;
int burnDelay;
int burnLow;
int flickDelay;
int flickLow;
int flutDelay;
int flutLow;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");


  
   musicPlayer.begin(); // initialise the music player
  SD.begin(CARDCS);    // initialise the SD card

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(1,1);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // button setup
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
  
  // candle setup

  flickerDepth = (burnDepth + flutterDepth) / 2.4;
  burnLow = grnHigh - burnDepth;
  burnDelay = (cycleTime / 2) / burnDepth;
  flickLow = grnHigh - flickerDepth;
  flickDelay = (cycleTime / 2) / flickerDepth;
  flutLow = grnHigh - flutterDepth;
  flutDelay = ((cycleTime / 2) / flutterDepth);

  strip.begin();
  strip.show();
  
  // start
  
  attachInterrupt(5, evil, RISING);

}

void loop() {
  
   // let there be light
   Serial.println("light start");
   burn(10);  
   flicker(5);
   burn(8);
   flutter(6);
   burn(3);
   on(10);
   burn(10);
   flicker(10);
   Serial.println("light finish");
}

void evil(){
  detachInterrupt(5);
  green();
  selectTrack();
  delay(20000);
  attachInterrupt(5, evil, RISING);
}

void green(){
  for (int i = 0; i <=8; i += 1){
    strip.setPixelColor(i, 0, 255, 0);
  }
  strip.show();    
  
}

void selectTrack(){
  randomNumber = random(1,6);
  Serial.println(randomNumber);
  fileName = "Evil" + String(randomNumber) + ".mp3";
  Serial.println(fileName);
  fileName.toCharArray(charBuf, 50);

  // Play one file, don't return until complete
  musicPlayer.playFullFile(charBuf);
}

// basic fire funciton - not called in main loop
void fire(int grnLow) {
  for (int grnPx = grnHigh; grnPx > grnLow; grnPx--) {
    int halfGrn = grnHigh - ((grnHigh - grnPx) / 2);
    int darkGrn = grnPx - 70;
    darkGrn = constrain(darkGrn, 5, 255);
    for (int index = 0; index <= 8; index += 7) {
      strip.setPixelColor(index, redPx-180, darkGrn, 0);
      strip.setPixelColor(index + 1, redPx-180, darkGrn, 0);
      strip.setPixelColor(index + 2, redPx-120, grnPx-50, bluePx-5);
      strip.setPixelColor(index + 3, redPx-60, grnPx-35, bluePx-2);
      strip.setPixelColor(index + 4, redPx, grnPx, bluePx);
      strip.setPixelColor(index + 5, redPx, grnPx, bluePx);
      strip.setPixelColor(index + 6, redPx, halfGrn, bluePx);
      strip.setPixelColor(index + 7, redPx, grnHigh, bluePx);
      strip.show();
      delay(fDelay);
    }
  }  
  for (int grnPx = grnLow; grnPx < grnHigh; grnPx++) {
    int halfGrn = grnHigh - ((grnHigh - grnPx) / 2);
    int darkGrn = grnPx-70;
    darkGrn = constrain(darkGrn, 5, 255);
    for (int index = 0; index <= 63; index += 7) {
      strip.setPixelColor(index, redPx-180, darkGrn, 0);
      strip.setPixelColor(index + 1, redPx-180, darkGrn, 0);
      strip.setPixelColor(index + 2, redPx-120, grnPx-50, bluePx-5);
      strip.setPixelColor(index + 3, redPx-60, grnPx-35, bluePx-2);
      strip.setPixelColor(index + 4, redPx, grnPx, bluePx);
      strip.setPixelColor(index + 5, redPx, grnPx, bluePx);
      strip.setPixelColor(index + 6, redPx, halfGrn, bluePx);
      strip.setPixelColor(index + 7, redPx, grnHigh, bluePx);
      strip.show();
      delay(fDelay);
    }
  }
}

// fire animation
void on(int f) {
  fRep = f * 1000;
  int grnPx = grnHigh - 6;
  strip.setPixelColor(0, redPx-180, grnPx-70, 0);
  strip.setPixelColor(1, redPx-180, grnPx-70, 0);
  strip.setPixelColor(2, redPx-120, grnPx-50, bluePx-5);
  strip.setPixelColor(3, redPx-60, grnPx-35, bluePx-2);
  strip.setPixelColor(4, redPx, grnPx, bluePx);
  strip.setPixelColor(5, redPx, grnPx, bluePx);
  strip.setPixelColor(6, redPx, grnPx, bluePx);
  strip.setPixelColor(7, redPx, grnHigh, bluePx);
  strip.show();
  delay(fRep);
}

void burn(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  for (int var = 0; var < fRep; var++) {
    fire(burnLow);
  }  
}

void flicker(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flickLow);
  }
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
  fire(burnLow);
}

void flutter(int f) {
  fRep = f * 8;  
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  fire(flickLow);
  fDelay = flutDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flutLow);
  }
  fDelay = flickDelay;
  fire(flickLow);
  fire(flickLow);
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
}


