import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port

void setup() 
{
  size(800, 200);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  printArray( Serial.list());
  String portName = Serial.list()[4];
  myPort = new Serial(this, portName, 19200);
  myPort.bufferUntil('\n'); 
  frameRate(60);
}

int read = 0;
int x = 0;

void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    String sVal = myPort.readStringUntil('\n');         // read it and store it in val
    //sVal = sVal.substring(0, sVal.length()-1);
    read++;
    if ( sVal != null & read > 2){
      sVal = trim(sVal);
      if ( sVal.length() > 0 && sVal.length() < 4) val = Integer.parseInt(sVal);
    }
  }
  
  x++;
  if ( x >= width ){
    x = 0;
    background(255);
  }
  if ( val < 800 ){
    background(255,0,0);
  } else { 
    background(255);
  }
  
  point(x,map(val, 0, 1024, 0, height));
}
