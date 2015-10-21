#include "ofApp.h"

int lastChanged = 0;
int changeRate = 5000;

//--------------------------------------------------------------
void ofApp::setup(){
    pattern.setup("svgs");

}

//--------------------------------------------------------------
void ofApp::update(){
//    if ( ofGetElapsedTimeMillis() - lastChanged > changeRate ){
//        pattern.redraw();
//        lastChanged = ofGetElapsedTimeMillis();
//    }
    pattern.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    pattern.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if ( key == 'r' ){
        pattern.redraw();
    }
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
