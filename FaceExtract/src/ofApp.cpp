#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

vector<string> toProcess;
bool bProcess = false;
ofImage processing;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	finder.setup("haarcascade_frontalface_alt2.xml");
	finder.setPreset(ObjectFinder::Accurate);
}

//--------------------------------------------------------------
void ofApp::update() {
    if ( bProcess && toProcess.size() != 0 ){
        if ( processing.loadImage(toProcess.front())){
            finder.update(processing);
        }
        toProcess.erase(toProcess.begin());
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if ( processing.isAllocated() ){
        processing.draw(0,0);
    }
	finder.draw();
	ofDrawBitmapStringHighlight(ofToString(finder.size()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
//    dragInfo.files;
    for ( auto & f : dragInfo.files ){
        toProcess.push_back(f);
    }
    bProcess = true;
}