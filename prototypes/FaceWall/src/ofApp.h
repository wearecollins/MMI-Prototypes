#pragma once

#include "ofMain.h"
#include "Face.h"
#include "SvgSampler.h"
#include "ofxCv.h"

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

        SvgSampler sampler;
    
        vector <Face> faces;
    
        ofxCv::FlowFarneback flow;
        ofVideoGrabber camera;
};
