#pragma once

#include "ofMain.h"
#include "ofPS3Eye.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxChromaKeyShader.h"

#include "Clip.h"
#include "RecordManager.h"
#include "ofxSpacebrew.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
        void draw();
        void exit();
        void keyPressed(int key);

    void setupCameras();
    void setupSpacebrew();
    
    vector<ofPS3Eye *> cameras;
    ofFbo videoTexture;
    
    int camWidth;
    int camHeight;
    int camFrameRate;
    
    ofxSyphonServer mainOutputSyphonServer;
    ofxPanel panel;
    ofxGuiGroup mainParams;
    bool bDrawGui;
    ofParameter<int> whichCameraTop;
    
    // fx
    ofShader effect;
    ofxToggle bUseBw, bUseChroma, bUpdateColor;
    
    // Chroma
    ofImage bg_image;
    ofxChromaKeyShader *chromakey;
    
    bool bShowGui;
    bool bUpdateBgColor;
    
    // spacebrew
    Spacebrew::Connection spacebrew;
    
    void onMessage( Spacebrew::Message & message );
    
    // recording
    RecordManager recorder;
    void onRecordComplete( string & complete );
};
