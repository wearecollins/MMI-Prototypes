#include "ofMain.h"
#include "ofApp.h"
#include "ofxLog4CppChannel.h"

//========================================================================
int main( ){
    ofSetupOpenGL(512,384,OF_WINDOW);			// <-------- setup the GL context

    // set up logging.
    // channels can be configured in bin/data/log4cpp.properties
    shared_ptr<ofxLog4CppChannel> log(new ofxLog4CppChannel());
    ofSetLoggerChannel(log);

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}
