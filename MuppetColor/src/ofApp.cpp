#include "ofApp.h"

int thresh = 10;

bool bSave = false;

//--------------------------------------------------------------
void ofApp::setup(){
    process_occlusion = true;
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
    depthShader.linkProgram();
    
    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
    irShader.linkProgram();
    
    kinect0.open(true, true, 0);
    // Note :
    // Default OpenCL device might not be optimal.
    // e.g. Intel HD Graphics will be chosen instead of GeForce.
    // To avoid it, specify OpenCL device index manually like following.
    // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
    
    kinect0.start();
    
    gr.setup(kinect0.getProtonect(), 2);
    
    // setup color trackin'
    
    contourFinder.setMinAreaRadius(10);
//    contourFinder.setMaxAreaRadius(200);
    trackingColorMode = ofxCv::TRACK_COLOR_RGB;
    
    ofXml settings;
    if ( settings.load("settings.xml") ){
        settings.setTo("color");
        targetColor.r = ofToInt(settings.getValue("r"));
        targetColor.g = ofToInt(settings.getValue("g"));
        targetColor.b = ofToInt(settings.getValue("b"));
        
//        settings.setToParent();
        thresh = settings.getValue("thresh", thresh);
        
        contourFinder.setTargetColor(targetColor, trackingColorMode);
        contourFinder.setThreshold(thresh);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect0.update();
    if (kinect0.isFrameNew()) {
        colorTex0.loadData(kinect0.getColorPixelsRef());
        depthTex0.loadData(kinect0.getDepthPixelsRef());
        irTex0.loadData(kinect0.getIrPixelsRef());
        
        depthTex0.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        gr.update(depthTex0, colorTex0, process_occlusion);
        
        
//        thresh = ofMap(mouseX, 0, ofGetWidth(), 0, 255);
        contourFinder.setTargetColor(targetColor, trackingColorMode);
        contourFinder.setThreshold(thresh);
        if ( kinect0.getColorPixelsRef().getWidth() != 0 )
            contourFinder.findContours(kinect0.getColorPixelsRef());
    }
    if ( bSave ){
        saveSettings();
        bSave = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofClear(0);
    
    if (colorTex0.isAllocated()) {
        colorTex0.draw(0, 0, 640, 360);
    }
    ofPushMatrix();
    ofScale(640./1920., 640./1920.);
    ofPushStyle();
    for(int i = 0; i < (int)contourFinder.size(); i++) {
        vector<ofPoint> l = contourFinder.getPolyline(i).getVertices();
        ofPath p;
        for ( auto & v : l ){
            p.lineTo(v);
        }
        p.setFillColor(ofColor(255,0,0));
        p.draw();
    }
    ofPopStyle();
    ofPopMatrix();
    ofSetColor(targetColor);
    ofDrawRectangle(0, 0, 50, 50);
    ofSetColor(255);
//    if (depthTex0.isAllocated()) {
//        
//        if (ofGetKeyPressed(' ')) {
//            gr.getRegisteredTexture(process_occlusion).draw(640, 0, 1024, 848);
//        } else {
//            irShader.begin();
//            irTex0.draw(640, 0, 1024, 848);
//            irShader.end();
//            ofPushStyle();
//            ofEnableAlphaBlending();
//            ofSetColor(255, 80);
//            gr.getRegisteredTexture(process_occlusion).draw(640, 0, 1024, 848);
//            ofPopStyle();
//        }
//        
//        depthShader.begin();
//        depthTex0.draw(0, 424, 512, 424);
//        depthShader.end();
//    }
    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
    ofDrawBitmapStringHighlight("Device Count : " + ofToString(ofxMultiKinectV2::getDeviceCount()), 10, 40);
    if (process_occlusion) {
        ofDrawBitmapStringHighlight("Process Occlusion Mask Enable", 10, 60);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        process_occlusion =  !process_occlusion;
    } else if (key == '+'){
        thresh++;
        contourFinder.setThreshold(thresh);
        bSave = true;
    } else if (key == '-'){
        thresh--;
        contourFinder.setThreshold(thresh);
        bSave = true;
    } else if ( key == 'm' ){
        if ( trackingColorMode == ofxCv::TRACK_COLOR_RGB) trackingColorMode = ofxCv::TRACK_COLOR_HSV;
        else trackingColorMode = ofxCv::TRACK_COLOR_RGB;
    }
}

//--------------------------------------------------------------
void ofApp::saveSettings(){
    ofXml settings;
    settings.addChild("color");
    settings.setTo("color");
    settings.addValue("r", (int) targetColor.r);
    settings.addValue("g", (int) targetColor.g);
    settings.addValue("b", (int) targetColor.b);
//    settings.setToParent();
    settings.addValue("thresh", (int) thresh);
    settings.save("settings.xml");
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
    if ( kinect0.getColorPixelsRef().getWidth() != 0 ){
        float scale = 1920./640.;
        targetColor = kinect0.getColorPixelsRef().getColor(x * scale, y * scale);
        bSave = true;
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
