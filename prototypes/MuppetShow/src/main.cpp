#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLWindowSettings settings;
    //    settings.setGLVersion(2, 1);  // Fixed pipeline
    settings.setGLVersion(3, 2);  // Programmable pipeline
    settings.width = 1024;
    settings.height = 768;
    ofCreateWindow(settings);
    
    ofRunApp(new ofApp());
    
}
