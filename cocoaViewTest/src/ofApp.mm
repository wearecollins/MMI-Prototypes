#import "ofApp.h"

@implementation ofApp

- (void)setup
{
    ofBackground(ofColor(255,255,255,0));
    [self setTranslucent:YES];
//    [[self window] setIgnoresMouseEvents:YES];
//    self->mouseEventsEnabled = NO;
//    self->_nextResponder = self.webView;
//    self.nextResponder = self.webView;
    
//    [webView load]
    string urlText = "http://127.0.0.1:8080";
    NSString * url = [NSString stringWithUTF8String:urlText.c_str()];
    [[self.webView mainFrame] loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:url]]];
    
    camera.setup(640,480);
}

- (void)update
{
    camera.update();
}

- (void)draw
{
//    ofSetColor(255,200);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    ofRotateZ(sin(ofGetElapsedTimef())*180);
    camera.draw(0,0, 320, 240);
}

- (void)exit
{
    
}

- (void)keyPressed:(int)key
{
    
}

- (void)keyReleased:(int)key
{
    
}

- (void)mouseMoved:(NSPoint)p
{
    
}

- (void)mouseDragged:(NSPoint)p button:(int)button
{
    
}

- (void)mousePressed:(NSPoint)p button:(int)button
{
    
}

- (void)mouseReleased:(NSPoint)p button:(int)button
{
    
}

- (void)windowResized:(NSSize)size
{
    
}

@end