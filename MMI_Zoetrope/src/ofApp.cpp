#include "ofApp.h"

float totalHeight = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
    ofBackground(0);
    
    ofDirectory seq;
    int n = seq.listDir( "sequences" );
    
    float y = 0;
    
    for ( int i=0; i<n; i++){
        tropes.push_back(new Zoetrope());
        tropes.back()->load(seq.getPath(i));
        tropes.back()->y = y;
        float r= ofRandomuf();
//        float h = r > .5 ? 480 * 2. : 480;
//        if ( r > .7 ) h = 480 * 3.;
        float h = 480;
        tropes.back()->setHeight( h );
        y += h;//tropes.back()->height;
    }
    
    totalHeight = y;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle( ofToString(ofGetFrameRate(), 3) );
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    float s = (ofGetHeight()) / totalHeight;
    ofScale(s,s);
    
    float y = 0;
    for ( auto * z : tropes ){
        z->y = y;
        z->draw(s);
        y += z->getHeight();//tropes.back()->height;
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
