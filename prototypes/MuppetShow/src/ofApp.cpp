#include "ofApp.h"

ofTexture kinectTexture;

bool bsaving = false;
bool bRewind = false;

vector<int> active;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(false);
    ofSetFrameRate(60.);
    ofBackground(0);
    ofXml settings;
    settings.load("settings.xml");
    
//    depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
//    depthShader.linkProgram();
//    
//    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
//    irShader.linkProgram();
#ifdef KINECT1
//    kinect.init();
//    kinect.open();
    contourFBO.allocate(640, 480);
    contourFBOOut.allocate(12, 10);
#else
//    kinect.open(true, true, 0, -1);
//    kinect.start();
//    contourFBO.allocate(640, 480);
#endif
    
    contourFBOOut.begin();
    ofClear(0);
    contourFBOOut.end();
    
    contourFBO.begin();
    ofClear(0);
    contourFBO.end();
    
    grid.setup(contourFBOOut, settings.getValue("folder"));
    
//    spacebrew.addSubscribe("grid", "range");
//    spacebrew.addSubscribe("grid1", "range");
//    spacebrew.addSubscribe("grid2", "range");
//    
//    spacebrew.connect();
    
    for ( int i=0; i<3; i++){
        active.push_back(-1);
    }
    
//    ofAddListener(spacebrew.onMessageEvent, this, &ofApp::onMessage);
}

bool bResize = false;

//--------------------------------------------------------------
void ofApp::update(){
    grid.update();
//    kinect.update();
    
    double min, max;
    
//    cv::Point minPoint, maxPoint;
    
//    if ( kinect.isFrameNew() ){
////        kinectTexture.loadData(kinect.getDepthPixels());
//        cv::Mat mat = ofxCv::toCv(kinect.getDepthPixels());
////        mat.convertTo(mat, CV_8U, .5, -250);
//        cv::minMaxLoc(mat, &min,
//                      &max, &minPoint, &maxPoint);
//    
//        contours.setThreshold(max * .8);
//        contours.findContours(mat);
//        
//        contourFBO.begin();
//        ofClear(0);
//        ofPushStyle();
//        
//        static ofPath path;
//        
//        for(int i = 0; i < (int)contours.getPolylines().size(); i++) {
//            auto & v = contours.getPolylines()[i].getVertices();
//            path.clear();
//            for ( auto & x : v ){
//                path.lineTo(x);
//            }
//            path.setFillColor(ofColor::white);
//            path.draw();
//        }
//        ofPopStyle();
//        contourFBO.end();
//    }
//    
//    contourFBOOut.begin();
//    ofClear(0);
////    ofClearAlpha();
////    ofSetColor(0, 1);
////    ofDrawRectangle(0, 0, contourFBOOut.getWidth(), contourFBOOut.getHeight());
//    ofSetColor(255);
//    contourFBO.draw(0,0,contourFBOOut.getWidth(), contourFBOOut.getHeight());
//    contourFBOOut.end();
    
    if (bResize){
        grid.resize();
        bResize = false;
    }
    
    if ( bRewind ){
        bRewind = false;
        grid.rewind();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    grid.draw();
//    depthShader.begin();
//    depthShader.end();
    
    if ( contourFBOOut.isAllocated() && ofGetKeyPressed('d')){
        contourFBOOut.draw(0,0, 640., 480.);
    }
    
    if (bsaving){
        ofSaveFrame();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'f' ) ofToggleFullscreen();
    if ( key == 'r' ) grid.reload();
    if ( key == '+'){
        static int activeIndex = 12;
        grid.setInactive(activeIndex);
        activeIndex++;
        activeIndex %= 119;
        grid.setActive(activeIndex);
    } else if ( key == 's'){
        bsaving = !bsaving;
    } else if ( key == 'R' ){
        bRewind = true;
    }
}

//--------------------------------------------------------------
// WEBSOCKET STUFF
//--------------------------------------------------------------
void ofApp::onMessage( Spacebrew::Message & m ){
    if ( m.name == "grid" ){
        // to-do:multiple people
        for ( int i=0; i<120;i++){
            bool found = false;
            for ( auto in : active ){
                if ( in == i ) found = true;
            }
            if ( !found ) grid.setInactive(i);
        }
        int dex = ofMap(m.valueRange(),0,1023,0,119,true);
        grid.setActive(dex);
        active[0] = dex;
    } else if ( m.name == "grid1" ){
        // to-do:multiple people
        for ( int i=0; i<120;i++){
            bool found = false;
            for ( auto in : active ){
                if ( in == i ) found = true;
            }
            if ( !found ) grid.setInactive(i);
        }
        int dex = ofMap(m.valueRange(),0,1023,0,119,true);
        grid.setActive(dex);
        active[1] = dex;
    } else if ( m.name == "grid2" ){
        // to-do:multiple people
        for ( int i=0; i<120;i++){
            bool found = false;
            for ( auto in : active ){
                if ( in == i ) found = true;
            }
            if ( !found ) grid.setInactive(i);
        }
        int dex = ofMap(m.valueRange(),0,1023,0,119,true);
        grid.setActive(dex);
        active[2] = dex;
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
    bResize = true;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
