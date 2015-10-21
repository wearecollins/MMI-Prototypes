#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
    void dragEvent(ofDragInfo dragInfo);
    
	ofxCv::ObjectFinder finder;
};
