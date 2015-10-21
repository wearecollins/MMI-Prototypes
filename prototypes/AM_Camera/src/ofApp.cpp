#include "ofApp.h"

float scale = 1.0;
float scaleInv = 1.0;
cv::Mat img, imgT;

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setup();
    cmt.setup();
    cmt.getCMT().thrConf = .5;
    cmt.getCMT().thrRatio = .9;
}

//--------------------------------------------------------------
void ofApp::update(){
    bool bNewFrame = kinect.update();
    
    if ( bNewFrame ){
        bool bUseDepth = false;
        if ( bUseDepth ){
            img = ofxCv::toCv(kinect.getDepthPixelsRef());
            img.convertTo(img, CV_8U, .5, -250);//ofMap(mouseX, 0, ofGetWidth(), 0, .5),ofMap(mouseY, 0, ofGetHeight(), 0, -255.));
            cout << ofMap(mouseX, 0, ofGetWidth(), 0, .5) <<":" <<ofMap(mouseY, 0, ofGetHeight(), 0,  -255.) << endl;
            
            ofxCv::invert(img, img);
            ofxCv::copyGray(img, img);
            
        } else {
            scale = (float) ofGetWidth() / 1920.f;
            scaleInv = 1./scale;
            imgT = ofxCv::toCv(kinect.getPixelsRef());
            ofxCv::copyGray(imgT, img);
            img.convertTo(img, -1, 1.5,-150.);
        }
        
        cmt.updateCv(img);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // debug
    static ofImage dImg;
    ofxCv::toOf(img, dImg);
    dImg.update();
    
//    scale = (float) ofGetWidth() / 1920.f;
    ofPushMatrix();
    ofScale(scale, scale);
    dImg.draw(0,0);
    //kinect.draw();
    cmt.draw();
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

///--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    cmt.mouseDragged(x * scaleInv, y * scaleInv);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    float s = (float) 1920.f/ofGetWidth();
    cmt.mousePressed(x * scaleInv, y * scaleInv);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    float s = (float) 1920.f/ofGetWidth();
    cmt.mouseReleased(x * scaleInv, y * scaleInv);
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
