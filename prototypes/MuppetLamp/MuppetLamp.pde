OPC opc;

void setup()
{
  size(800, 200);
  
  colorMode(HSB, 255);
  
  // Connect to the local instance of fcserver
  opc = new OPC(this, "127.0.0.1", 7890);

  // Map one 64-LED strip to the center of the window
  opc.ledStrip(0, 64, width/2, height/2, width / 42.0, 0, false);
  frameRate(60);
}

void draw()
{
  color c = color(map(mouseX, 0, width, 0, 255), 255, 255);
  background(c);
}

