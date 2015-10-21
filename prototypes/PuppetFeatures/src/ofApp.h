#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        Spacebrew::Connection spacebrew;
        ofSerial device;
};
