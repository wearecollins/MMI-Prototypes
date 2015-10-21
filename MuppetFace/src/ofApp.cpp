#include "ofApp.h"
#include "SvgSampler.h"

bool debug = false;
bool last = false;

Face muppet;

SvgSampler sampler;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    camera.initGrabber(640,480);
    
    finderFace.setup("haarcascade_frontalface_alt2.xml");
    finderFace.setPreset(ofxCv::ObjectFinder::Fast);
    finderEyes.setup("haarcascade_eye_tree_eyeglasses.xml");
    finderEyes.setPreset(ofxCv::ObjectFinder::Accurate);
    
    finderFace.getTracker().setSmoothingRate(.1);
    finderEyes.getTracker().setSmoothingRate(.1);
    
    sampler.load(ofToDataPath("colors.svg"));
    muppet.setup();
    muppet.color = sampler.getRandomColor();
}

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    if ( camera.isFrameNew() ){
        finderFace.update(camera);
        if ( finderFace.size() > 0 ){
            if ( !last ){
                muppet.color = sampler.getRandomColor();
                muppet.reset();
                last = true;
            }
            cv::Mat mat = ofxCv::toCv(camera);
            unsigned int l = finderFace.getTracker().getLabelFromIndex(0);
            cv::Rect r = finderFace.getTracker().getSmoothed(l);
            mat = cv::Mat(mat, r);
            finderEyes.update(mat);
            
            if ( finderEyes.size() > 0 ){
                muppet.eye1 = finderEyes.getObjectSmoothed(0);
            } else {
                muppet.eye1.height = 0;
            }
            
            if ( finderEyes.size() > 1 ){
                muppet.eye2 =finderEyes.getObjectSmoothed(1);
            } else {
                muppet.eye2.height = 0;
            }
            muppet.eye2.y     = muppet.eye1.y;
            muppet.eye2.width = muppet.eye1.width;
            muppet.eye2.height = muppet.eye1.height;
       
        
        } else {
            last = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( ofGetMousePressed() ){
        camera.draw(0,0);
        finderFace.draw();
        ofPushMatrix();
        if ( finderFace.size() > 0 ){
//            unsigned int l = finderFace.getTracker().getLabelFromIndex(0);
            ofTranslate(finderFace.getObjectSmoothed(0).x, finderFace.getObjectSmoothed(0).y);
            finderEyes.draw();
        }
        ofPopMatrix();
    }
    
    if ( finderFace.size() > 0 ){
        muppet.set(finderFace.getObjectSmoothed(0));
    }
    muppet.draw();
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
