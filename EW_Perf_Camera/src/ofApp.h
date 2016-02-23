#pragma once

#include "ofMain.h"
//#define USE_VG
#include "ofPS3Eye.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxChromaKeyShader.h"

#include "Clip.h"
#include "RecordManager.h"
#include "ofxSpacebrew.h"

#include "WebsocketStreamer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
        void draw();
        void exit();
        void keyPressed(int key);

    void setupCameras();
    void setupSpacebrew();
    
#ifdef USE_VG
    vector<ofVideoGrabber *> cameras;
#else
    vector<ofPS3Eye *> cameras;
#endif
    ofFbo videoTexture, cameraTop, cameraSide;
    ofBufferObject pixelBufferBack[2];
    ofBufferObject pixelBufferFront[2];
    
    int camWidth;
    int camHeight;
    int camFrameRate;
    
    ofxSyphonServer mainOutputSyphonServer;
    WebsocketStreamer websocketStream;
    
    ofxPanel panel;
    ofxGuiGroup mainParams;
    bool bDrawGui;
    ofParameter<int> whichCameraTop;
    
    // fx
    ofShader effect;
    ofxToggle bUseBw, bUseChroma, bUpdateColor, bUsePb, bUseWs;
    
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
