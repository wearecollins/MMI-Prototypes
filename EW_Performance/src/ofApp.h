#pragma once

#include "ofMain.h"
#include "RecManager.h"
#include "ClipManager.h"

#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // bg
        ofImage background;
    
        // ofxUI
        ofxPanel gui;
		ofParameter<bool> useOverlay, doRecord;
		ofParameter<ofVec2f> lScreenPos,rScreenPos;
		ofParameter<float> min, max;
		ofParameter<bool> screenRight;

        // clipz
        ClipManager clips;
    
        // recording
        RecManager recorder;
        void donePlayback();

		ofVideoGrabber cameraLeft, cameraRight;

		ofFbo leftScreen, rightScreen, saver;
    
        ofMesh test;
};
