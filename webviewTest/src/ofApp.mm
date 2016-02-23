#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    //loading a web URL:
    webView.setSize(1280, 720);
    ofSetWindowShape(1280, 720);
    webView.setPosition(0,0);
    
    ofAddListener(webView.LOAD_URL,this,&ofApp::onPageLoad);
    
    //doesn't seem to support WebGL
    //webView.loadURL("http://threejs.org/examples/#webgl_buffergeometry_rawshader");
    //webView.loadURL("http://localprojects.net");
    webView.loadURL("http://127.0.0.1:8080");
    
    webTexture.allocate(webView.getWidth(),webView.getHeight(),GL_RGBA);
    webTexture.clear();
    
    webView.setDrawsBackground( true );
}

//--------------------------------------------------------------
void ofApp::onPageLoad(WebViewEvent &e){
    cout<<"This stuff is being loaded now "<<e.URL<<endl;
    
    //if you want to block it..maybe to extract javascript variables
    //webView.setAllowPageLoad(false);
    
};

//--------------------------------------------------------------
void ofApp::update(){
    webTexture.clear();
    webView.toTexture(&webTexture);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    if ( webTexture.isAllocated() )
        webTexture.draw(0,webView.getY(),webView.getWidth(),webView.getHeight());
    
    ofSetWindowTitle(ofToString((int)ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'f' ) ofToggleFullscreen();
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
