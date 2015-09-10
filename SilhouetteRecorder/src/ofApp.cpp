#include "ofApp.h"

static bool bDrawGui = true;

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    gui.setup();
    gui.add(kinect.kinectLow);
    gui.add(kinect.kinectHigh);
    gui.add(kinect.depthColorLow);
    gui.add(kinect.depthColorHigh);
    gui.add(kinect.bDrawColor);
    gui.add(kinect.bDrawDepth);
    gui.add(kinect.bDrawIr);
    gui.add(saving.set("Saving", false));
    gui.loadFromFile("settings.xml");
    renderer.allocate(KINECT2_DEPTH_X, KINECT2_DEPTH_Y, GL_RGB, 8);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    if ( saving ){
        renderer.begin();
        ofClear(0);
        kinect.draw();
        renderer.end();
        ofPixels out;
        renderer.readToPixels(out);
        ofSaveImage(out, ofGetTimestampString() + ".png");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw();
    
    if ( bDrawGui ){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ) bDrawGui = !bDrawGui;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
