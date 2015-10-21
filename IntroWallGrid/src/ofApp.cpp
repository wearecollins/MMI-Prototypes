#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bShowGui = true;
    ofSetFrameRate(60.);
    
    grid = new SvgGrid();
    
    gui.setup();
    gui.add( grid->parameters );
    gui.add(blendMode.set("Blend mode", 0, 0, 5));
    grid->setup( "intro.svg" );
    
    ofDirectory dir;
    string directory;
    
    auto r = ofSystemLoadDialog("Location of videos", true);
    if ( r.bSuccess ){
        directory = r.getPath();
    } else {
        ofExit();
    }
    
    int n = dir.listDir(directory);
    auto f = dir.getFiles();
    ofRandomize(f);
    
    for ( auto & file : f ){
        auto * v = new ofVideoPlayer();
//        int ind = i;
//        if ( ind < 0 ){
//            ind = 0;
//        }
        bool l = v->load( file.getAbsolutePath() );
        if ( l ){//&& i >= 0 ){
            videos.push_back(v);
            v->play();
            v->setFrame(0);
            v->setVolume(0);
            grid->addTexture(v);
            v->setPaused(true);
        }
    }
    
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){
    grid->update();
    
    auto f = getTime(true);
    bool bNext = true;
    
    for ( auto * v : videos ){
        v->update();
        if ( bNext ) v->nextFrame();
        if ( v->getCurrentFrame() +1 >= v->getTotalNumFrames() ){
            v->setFrame(0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableBlendMode((ofBlendMode) blendMode.get() );
    
    ofPushMatrix();
    grid->draw();
    ofPopMatrix();
    if ( bShowGui ){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g') bShowGui = !bShowGui;
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
