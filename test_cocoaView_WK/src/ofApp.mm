#import "ofApp.h"

@implementation ofApp

@synthesize webView;

- (void)setup
{
    ofSetFrameRate(60);
    ofBackground(ofColor(255,255,255,0));
    [self setTranslucent:YES];

//    WKWebViewConfiguration * config = [WKWebViewConfiguration init];
    self.webView = [[WKWebView alloc] initWithFrame:[self frame]];//] configuration:config];
    string urlText = "http://localhost:8080";
    NSString * url = [NSString stringWithUTF8String:urlText.c_str()];
    [self.webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:url]]];

    [self.webView setValue:@YES forKey:@"drawsTransparentBackground"];

    [self addSubview:self.webView];
    
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
    camera.draw(0,0, 640,480);
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
    [self.webView setFrame:[self frame]];
}

@end