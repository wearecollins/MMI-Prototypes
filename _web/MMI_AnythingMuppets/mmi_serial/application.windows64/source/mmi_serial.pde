import processing.serial.*;
import cc.arduino.*;
import spacebrew.*; 

Spacebrew sb;

String server="localhost";  // the ip address goes in the quotes. i.e. “255.255.255.0”
String name="Arch Firmata";
String description ="This app sends out inputs received from Arduino analog and digital inputs";

Arduino arduino;
color off = color(4, 79, 111);
color on = color(84, 145, 158);

void setup(){
  size(800,600);
  
  // instantiate the spacebrew Connection variable
  sb = new Spacebrew( this );

  // Modify this line, by changing the "0" to the index of the serial
  // port corresponding to your Arduino board (as it appears in the list
  // printed by the line above).
  arduino = new Arduino(this, Arduino.list()[0], 57600);

  // Alternatively, use the name of the serial port corresponding to your
  // Arduino (in double-quotes), as in the following line.
  //arduino = new Arduino(this, "/dev/tty.usbmodem621", 57600);
  
  // Set the Arduino analog pin Spacebrew Publishers
  // Set the Arduino digital pins as inputs and create Spacebrew Publishers for digital reads
  for (int i = 0; i <= 13; i++) {
    arduino.pinMode(i, Arduino.INPUT);
    sb.addPublish( "button_"+i+"_pressed", "boolean", false );
  }

  // Set the Arduino analog pin Spacebrew Publishers
  for (int i = 0; i< 1; i++) {
    sb.addPublish( "analog_"+i+"_val", "range", arduino.analogRead(i) );
  }
  
  sb.connect( server, name, description);
}

void draw() {
  background(off);
  stroke(on);

  // Draw a filled box for each digital pin that's HIGH (5 volts).
  for (int i = 0; i <= 13; i++) {
    if (arduino.digitalRead(i) == Arduino.HIGH) {
                sb.send( "button_"+i+"_pressed", true );
      fill(on);
    }
    else{
                sb.send( "button_"+i+"_pressed", false );
     fill(off);
    }

    rect(420 - i * 30, 30, 20, 20);
  }

  // Draw a circle whose size corresponds to the value of an analog input.
  noFill();
 for (int i = 0; i < 1; i++) {
    ellipse(280 + i * 30, 240, arduino.analogRead(i) / 16, arduino.analogRead(i) / 16);
    if (arduino.analogRead(i) > 0) {
      sb.send( "analog_"+i+"_val", arduino.analogRead(i));
    }
  }
}