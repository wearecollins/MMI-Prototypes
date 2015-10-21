import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import cc.arduino.*; 
import spacebrew.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class mmi_serial extends PApplet {



 

Spacebrew sb;

String server="localhost";  // the ip address goes in the quotes. i.e. \u201c255.255.255.0\u201d
String name="Arch Firmata";
String description ="This app sends out inputs received from Arduino analog and digital inputs";

Arduino arduino;
int off = color(4, 79, 111);
int on = color(84, 145, 158);

public void setup(){
  
  
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

public void draw() {
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
  public void settings() {  size(800,600); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "mmi_serial" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
