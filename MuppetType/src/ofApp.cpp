#include "ofApp.h"

ofColor colorA, colorB;

//--------------------------------------------------------------
void ofApp::setup(){
    vector<string> lists;
    lists.push_back("samandfriends.txt");
    lists.push_back("sesamestreet.txt");
    lists.push_back("muppets.txt");
    lists.push_back("immersive.txt");
//    ofSetVerticalSync(false);
    ofSetFrameRate(60);
    
    float height = 30;
    font.load("maax.otf", height);
    
    int dim = ofGetHeight() / height;
    
    float y = 0;
    
    
    vector<string> allLines;
    int ind = 0;
    
    for ( auto & l : lists ){
        ofFile lineFile;
        lineFile.open(ofToDataPath(l));
        string lineBuff = lineFile.readToBuffer().getText();
        vector<string> split = ofSplitString(lineBuff, "\n");
        for ( auto & s : split ){
            s = ofToString(ind) + ":" + s;
        }
        ind++;
        allLines.insert(allLines.begin(), split.begin(), split.end());
    }
    
    for ( int i=0; i<dim; i++){
        lines.push_back( Line() );
        ofRandomize(allLines);
        lines.back().setup(allLines, font);
        lines.back().y = y;
        lines.back().x = ofRandom(-1000);
        y += height * 2.0;
    }
    
    ofBackground(0);
    
    colorA.setSaturation(200);
    colorA.setBrightness(200);
    colorA.setHue(ofRandom(255));
    
    colorB.setSaturation(200);
    colorB.setBrightness(200);
    colorB.setHue(ofWrap(colorA.getHue() * 1.5, 0, 255));
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(colorA, colorB, OF_GRADIENT_LINEAR);
    
    for ( auto & l : lines ){
        l.draw();
    }
    
    colorA.setHue( ofWrap(colorA.getHue() - .5, 0, 255) );
    colorB.setHue( ofWrap(colorB.getHue() - .5, 0, 255) );
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
