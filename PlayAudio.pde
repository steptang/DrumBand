import processing.serial.*;
import processing.sound.*;

/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
SoundFile snare_file;
SoundFile hightoms_file;
SoundFile floortom_file;
SoundFile highhat_file;
SoundFile crashcymbal_file;

void setup() 
{
  size(200, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 115200);
  snare_file = new SoundFile(this, "drum_snare.mp3");
  hightoms_file = new SoundFile(this, "top_toms.wav");
  floortom_file = new SoundFile(this, "floor_tom.wav");
  highhat_file = new SoundFile(this, "high_hat.mp3");
  crashcymbal_file = new SoundFile(this, "crash_cymbal.mp3");
}

void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
  }
  background(255);             // Set background to white
  if (val == 1) {     // If the serial value is 0,
    fill(51, 102, 255);                  // set fill to blue
    snare_file.play();
    val = 0;
    myPort.clear();
  } 
  else if (val == 2){
    fill(102,41,255);                  // set fill to purple
    highhat_file.play();
    val = 0;
    myPort.clear();
  }else if (val == 3){
    fill(204,51,255);                  // set fill to magenta
    hightoms_file.play();
    val = 0;
    myPort.clear();
  }else if (val == 4){
    fill(51,204,255);                  // set fill to light blue
    crashcymbal_file.play();
    val = 0;
    myPort.clear();
  }else if (val == 5){
    fill(255,51,203);                  // set fill to pink
    floortom_file.play();
    val = 0;
    myPort.clear();
  }
  else{                       // If the serial value is not 0
    fill(0);                   // set fill to black
  }
  rect(50, 50, 100, 100);
}



/*

// Wiring / Arduino Code
// Code for sensing a switch status and writing the value to the serial port.

int switchPin = 4;                       // Switch connected to pin 4

void setup() {
  pinMode(switchPin, INPUT);             // Set pin 0 as an input
  Serial.begin(9600);                    // Start serial communication at 9600 bps
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {  // If switch is ON,
    Serial.write(1);               // send 1 to Processing
  } else {                               // If the switch is not ON,
    Serial.write(0);               // send 0 to Processing
  }
  delay(100);                            // Wait 100 milliseconds
}

*/