#include "ofApp.h"

int nScripts = 13;

//--------------------------------------------------------------
void ofApp::setup(){
    for (int i=0; i<nScripts; i++){
        scripts.push_back(Script());
        scripts.back().setup(300,400);
        scripts.back().rotation = ofRandom(-5,5);
        if ( ofRandom(100) > 70 ){
            scripts.back().rotation = ofRandom(-15,15);
        }
        scripts.back().x += ofRandom(-20,20);
        scripts.back().y += ofRandom(-20,20);
    }
    ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update(){
    for ( auto & s : scripts ){
        s.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.);
    for ( auto & s : scripts ){
        s.draw();
    }
    ofPopMatrix();
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
    
    for ( int i= scripts.size()-1; i>=0; i--){
        scripts[i].mouseMoved(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    for ( int i= scripts.size()-1; i>=0; i--){
        if ( scripts[i].mousePressed(x,y) ){
            break;
        }
    }
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
