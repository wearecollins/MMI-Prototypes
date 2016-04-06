#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"
#include "ofxGui.h"
#include "json.h"

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

        //websocket methods
        void onConnect( ofxLibwebsockets::Event& args ){}
        void onOpen( ofxLibwebsockets::Event& args ){}
        void onClose( ofxLibwebsockets::Event& args ){}
        void onIdle( ofxLibwebsockets::Event& args ){}
        void onMessage( ofxLibwebsockets::Event& args ){}
        void onBroadcast( ofxLibwebsockets::Event& args ){}

        ofxFloatSlider rotation;
        ofxToggle switch1;
        ofxToggle switch2;
        ofxPanel gui;
        void rotationChanged(float &rotation);
        void switch1Changed(bool &toggle);
        void switch2Changed(bool &toggle);
private:
        ofVideoPlayer fingerMovie;
        ofxLibwebsockets::Server webSocketServer;
};
