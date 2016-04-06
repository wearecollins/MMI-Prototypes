#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    rotation.addListener(this, &ofApp::rotationChanged);
    switch1.addListener(this, &ofApp::switch1Changed);
    switch2.addListener(this, &ofApp::switch2Changed);

    ofLogVerbose() << "a verbose print";
    ofLogNotice() << "a regular notice print";
    ofLogWarning() << "uh oh, a warning";
    ofLogError() << "oh no, an error occurred!";
    ofLogFatalError() << "accckkk, a fatal error!!";

    ofLogFatalError("ofApp.update") << "another fatal?";

    fingerMovie.load("assets/fingers.mov");
    fingerMovie.play();

    ofxLibwebsockets::ServerOptions options = 
        ofxLibwebsockets::defaultServerOptions();
    options.port = 9091;
    bool connected = webSocketServer.setup(options);
    if (!connected){
      ofLogWarning() << 
        "websocket server not connected, probably address in use: " << 
        options.port;
    }
    webSocketServer.addListener(this);

    gui.setup();
    gui.add(rotation.setup("rotation", 0, 0, 180));
    gui.add(switch1.setup("switch1", false));
    gui.add(switch2.setup("switch2", false));
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.update();
    if (fingerMovie.isFrameNew()){
        ofBuffer buffer;
        ofSaveImage(fingerMovie.getPixels(), buffer, OF_IMAGE_FORMAT_JPEG);
        webSocketServer.sendBinary(buffer.getData(), buffer.size());
    }
}

void ofApp::rotationChanged(float &rotation){
  if (switch1 != (rotation > 90)){
    switch1 = (rotation > 90);
  }
  if (switch2 != (rotation > 175)){
    switch2 = (rotation > 175);
  }
}

void ofApp::switch1Changed(bool &toggle){
  Json::Value root;
  Json::FastWriter writer;

  root["event"]["name"] = "switch1";
  root["event"]["detail"] = toggle;

  webSocketServer.send(writer.write(root));
}

void ofApp::switch2Changed(bool &toggle){
  Json::Value root;
  Json::FastWriter writer;

  root["event"]["name"] = "switch2";
  root["event"]["detail"] = toggle;

  webSocketServer.send(writer.write(root));
}

//--------------------------------------------------------------
void ofApp::draw(){
    fingerMovie.draw(0, 0, ofGetWidth(), ofGetHeight());
    gui.draw();
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
