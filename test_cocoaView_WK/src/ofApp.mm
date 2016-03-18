#import "ofApp.h"

@implementation ofApp

@synthesize webView;
@synthesize delegate;

- (void)setup
{
    ofSetFrameRate(60);
    ofBackground(ofColor(255,255,255,0));

    self.webView = [[WKWebView alloc] initWithFrame:[self frame]];//] configuration:config];
    
    string urlText = "http://localhost:8080";
    NSString * url = [NSString stringWithUTF8String:urlText.c_str()];
    
    self.delegate = [[WKDelegate alloc] init];
    
    // this is silly
    self.delegate.isLoaded = &self->isLoaded;
    
    [self.webView setNavigationDelegate:self.delegate];
    
    self->isLoaded = false;
    
    [self.webView loadRequest:[[NSURLRequest alloc] initWithURL:[NSURL URLWithString:url]]];
    
    [self setWantsLayer:YES];
    [self.webView setWantsLayer:YES];
    [self setTranslucent:YES];
    [self.webView setValue:@YES forKey:@"drawsTransparentBackground"];
    
    [[self superview] addSubview:self positioned:NSWindowAbove relativeTo:nil];
    [[self superview] addSubview:self.webView positioned:NSWindowAbove relativeTo:nil];
    
    camera.setup("b09d0100eefbf3");
}

- (void)update
{
    if ( self->isLoaded ){
        self->isLoaded = false;
    }
}

- (void)draw
{
//    ofSetColor(255,200);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    ofRotateZ(sin(ofGetElapsedTimef())*180);
    camera.draw(0,0);
    
    static bool bReady = false;
    if (!bReady){
        ofSetWindowShape(640, 481);
        bReady = true;
    }
}

- (void)exit
{
    
}

- (void)keyPressed:(int)key
{
    
}

- (void)keyReleased:(int)key
{
    if ( key =='r'){
        [self.webView reload:nil];
    }
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