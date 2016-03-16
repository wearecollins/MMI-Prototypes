#include "ofApp.h"

vector<string> prompts;
int promptIndex = 0;
int lastChanged = 0;
int changeRate = 5000;
ofTrueTypeFont font;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(54, 54, 54);
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    soundStream.listDevices();
    
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    
    int bufferSize = 256;
    
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    video.loadMovie("kermit_sync.mp4");
    video.stop();
    
    prompts.resize(6);
    prompts[0] = "Sound scary";
    prompts[1] = "Sound happy";
    prompts[2] = "Sound sad";
    prompts[3] = "Sound weird";
    prompts[4] = "Sound old";
    prompts[5] = "Sound young";
    
    font.loadFont("agro.ttf", 30);
}

//--------------------------------------------------------------
void ofApp::update(){
    if ( ofGetElapsedTimeMillis() - lastChanged > changeRate ){
        lastChanged = ofGetElapsedTimeMillis();
        promptIndex = floor(ofRandom(prompts.size()));
        
    }
    //lets scale the vol up to a 0-1 range
    scaledVol = ofMap(smoothedVol, 0.0, 0.03, 0.0, 1.0, true);
    
    video.setPosition(scaledVol);
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    float scale = (float) ofGetWidth() / video.getWidth();
    ofPushMatrix();
    ofScale(scale, scale);
    video.draw(0, 0);
    ofPopMatrix();
    
    font.drawString(prompts[promptIndex], ofGetWidth()/2.0 - font.stringWidth(prompts[promptIndex])/2.0, ofGetHeight() - font.stringHeight(prompts[promptIndex]) * 2.0);
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.9;
    smoothedVol += 0.1 * curVol;
    
    bufferCounter++;
    
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
