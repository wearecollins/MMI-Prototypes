#include "ofMain.h"
#include "ofApp.h"

//========================================================================
//int main( ){
//	ofSetupOpenGL(640,480,OF_WINDOW);			// <-------- setup the GL context
//
//	// this kicks off the running of my app
//	// can be OF_WINDOW or OF_FULLSCREEN
//	// pass in width and height too:
//	ofRunApp(new ofApp());
//
//}

int main()
{
    ofGLWindowSettings settings;
//    settings.setGLVersion(3, 2);
    settings.width = 640;
    settings.height = 480;
    ofCreateWindow(settings);
    ofRunApp(std::make_shared<ofApp>());
}