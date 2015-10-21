#pragma once

#include "ofMain.h"
#include "ofPS3Eye.h"
#include "ofxSyphon.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
        void draw();
        void exit();
        void keyPressed(int key);

    
    vector<ofPS3Eye *> cameras;
    ofFbo videoTexture;
    
    int camWidth;
    int camHeight;
    int camFrameRate;
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxPanel panel;
    bool bDrawGui;
    
};
