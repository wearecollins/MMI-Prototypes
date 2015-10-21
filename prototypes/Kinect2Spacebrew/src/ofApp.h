#pragma once

#include "ofMain.h"

#include "ofxKinectV2.h"
#include "ofxCv.h"
#include "ofxUI.h"

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        ofxKinectV2 kinect;
        ofTexture   debugTexture;
        ofxUICanvas * gui;
    
        ofxCv::ContourFinder blobFinder;
        ofxCv::RunningBackground thresholded;
        ofImage background;
    
        vector<ofSoundPlayer> keys;
};
