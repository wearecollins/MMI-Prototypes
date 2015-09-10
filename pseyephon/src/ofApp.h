#pragma once

#include "ofMain.h"
#include "ofxPS3EyeGrabber.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
        void draw();
        void exit();
        void keyPressed(int key);

    
    ofxPS3EyeGrabber vidGrabber;
    ofTexture videoTexture;
    
    int camWidth;
    int camHeight;
    int camFrameRate;
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxPanel panel;
    bool bDrawGui;
    
    ofParameter<float> gain, exposure, sharpness, contrast, brightness, hue, redBalance, blueBalance;
};
