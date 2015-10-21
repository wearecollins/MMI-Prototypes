#include "ofApp.h"

float near, far;
float thresh;
float big, small;

int ageThresh = 100;

bool bUseBackground = true;

int which = 0;

ofVec3f pos, rect;

map<unsigned int, cv::Rect> goodLabels;

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.open();
    
    near = 0.;
    far = 1000;
    thresh = 100;
    
    gui = new ofxUICanvas(0,0, ofGetWidth()/2.0, ofGetHeight());
//    gui->addSlider("near", &kinect.minDistance, 0., 1000.);
    gui->addSlider("Kinect near", 0., 10000., &near);
    gui->addSlider("Kinect far", 0., 10000., &far);
    gui->addSlider("Threshold", 0., 255., &thresh);
    gui->addSlider("Blob min", 0.0, 1000, &small);
    gui->addSlider("Blob max", 0.0, 1000, &big);
    gui->addIntSlider("Blob age", 0.0, 10000, &ageThresh);
    gui->addToggle("Use background", &bUseBackground);
    gui->add2DPad("ROI XY", ofVec3f(0,512), ofVec3f(0,424), &pos, 256, 212);
    gui->add2DPad("ROI WH", ofVec3f(0,512), ofVec3f(0,424), &rect, 256, 212);
    gui->loadSettings("settings.xml");
    
    blobFinder.setThreshold(thresh);
    
    ofDirectory dir;
    int n = dir.listDir("casio");
    
    for ( int i=0; i<n; i++){
        keys.push_back(ofSoundPlayer());
        keys.back().loadSound(dir.getPath(i));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    kinect.minDistance.set(near);
    kinect.maxDistance.set(far);
    blobFinder.setMinArea(small);
    blobFinder.setMaxArea(big);
    blobFinder.setThreshold(thresh);
    
    if ( kinect.isFrameNew() ){
        
        ofPixels trackPixels = kinect.getDepthPixels();
        trackPixels.crop(pos.x, pos.y, rect.x, rect.y);
        debugTexture.loadData(trackPixels);//kinect.getDepthPixels());
        
        thresholded.update(trackPixels, background.getPixelsRef());
        if ( bUseBackground ) blobFinder.findContours(thresholded.getForeground());
        else blobFinder.findContours(trackPixels);//kinect.getDepthPixels());
//        if ( bUseBackground) blobFinder.findContours(thresholded.getForeground());// kinect.getDepthPixels());
    }
    
    static int lastSize = 0;
    
    auto & t = blobFinder.getTracker();
    for ( auto & r : t.getCurrentLabels()){
        if ( t.getAge(r) > ageThresh ){
            goodLabels[r] = blobFinder.getBoundingRect(r);
            cout << t.getAge(r) << endl;
        }
    }
    
    if ( goodLabels.size() > 0 ){
        for ( auto it = goodLabels.begin(); it != goodLabels.end(); it++){
            if ( !t.existsCurrent(it->first)){
                goodLabels.erase(it->first);
            }
        }
    }
    
    // to-do: get oldest
    if ( goodLabels.size() > lastSize ){
        keys[which].play();
        which++;
        if ( which >= keys.size() ) which = 0;
    } else if (goodLabels.size() < lastSize ){
        keys[which].play();
        which --;
        if ( which < 0 ) which = keys.size() - 1;
    }
    lastSize = goodLabels.size();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( debugTexture.isAllocated()){
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2.0,0);
        debugTexture.draw(0,0);
        blobFinder.draw();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 's' ){
        gui->saveSettings("settings.xml");
    } else if ( key == ' ' ){
        thresholded.reset();
    }
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
