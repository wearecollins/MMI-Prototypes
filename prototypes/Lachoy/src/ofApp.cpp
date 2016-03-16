#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    // ---
    _mapping = new ofxGemcutter();
    _mapping->setup(ofGetWidth(), ofGetHeight());
    
    image.load("lachoy.png");
}

//--------------------------------------------------------------
void ofApp::update(){
    
    _mapping->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // ---
    _mapping->bind();
    ofSetColor(255);
    // draw a test pattern
    //    _mapping->chessBoard();
    
    image.draw(0,0);
    
    _mapping->unbind();
    
    
    // ---
    _mapping->draw();
    //_mapping->drawFbo();
    
    
    //    ofBeginShape();
    //    ofFill();
    //    ofSetColor(0, 255, 0);
    //    for (int i = 0; i < _mapping->getMaskShapes()[0]->size(); i++) {
    //        ofVertex(_mapping->getMaskShapes()[0]->getVertices()[i].x, _mapping->getMaskShapes()[0]->getVertices()[i].y);
    //    }
    //    ofEndShape(true);
    
    
    // Draw some instructions.
    /*
     ofSetColor(0);
     ofDrawBitmapString("'m' open the mapping controls.\n", 20, 20);
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}