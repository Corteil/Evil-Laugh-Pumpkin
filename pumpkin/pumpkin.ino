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


Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(RESET, CS, DCS, DREQ, CARDCS);

int randomNumber = 0;
String fileName ="";
char charBuf[50];


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
  
  randomNumber = random(1,6);
  Serial.println(randomNumber);
  fileName = "Evil" + String(randomNumber) + ".mp3";
  Serial.println(fileName);
  fileName.toCharArray(charBuf, 50);

  // Play one file, don't return until complete
  musicPlayer.playFullFile(charBuf);
}

void loop() {
  randomNumber = random(1,6);
  Serial.println(randomNumber);
  fileName = "Evil" + String(randomNumber) + ".mp3";
  Serial.println("hello");
  Serial.println(fileName);
  fileName.toCharArray(charBuf, 50);

  // Play one file, don't return until complete
  musicPlayer.playFullFile(charBuf);
  delay(2000);
}

