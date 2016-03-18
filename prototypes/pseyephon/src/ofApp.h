#pragma once

#include "ofMain.h"
#include "ofPS3Eye.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxChromaKeyShader.h"

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
    
    // fx
    ofShader effect;
    ofxToggle bUseBw, bUseChroma, bUpdateColor;
    
    // Chroma
    ofImage bg_image;
    
    ofxChromaKeyShader *chromakey;
    
    bool bShowGui;
    bool bUpdateBgColor;
};
