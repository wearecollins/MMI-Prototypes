#pragma once

#include "ofMain.h"
#include "ofxCocoaGLView.h"
#import <WebKit/WebKit.h>

@interface ofApp : ofxCocoaGLView {
    WKWebView *webView;
    ofVideoGrabber camera;
}

- (void)setup;
- (void)update;
- (void)draw;
- (void)exit;

- (void)keyPressed:(int)key;
- (void)keyReleased:(int)key;
- (void)mouseMoved:(NSPoint)p;
- (void)mouseDragged:(NSPoint)p button:(int)button;
- (void)mousePressed:(NSPoint)p button:(int)button;
- (void)mouseReleased:(NSPoint)p button:(int)button;
- (void)windowResized:(NSSize)size;


@property (retain) WKWebView *webView;


@end
