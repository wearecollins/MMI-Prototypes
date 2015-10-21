#include "ofApp.h"

string saveLocation = "";

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(30.);
    gui.setup();
    gui.add(kaleido.parameters);
    gui.add(triangles.parameters);
    gui.add(grid.parameters);
    gui.add(drawMode.set("Draw mode", 0, 0, 2));
    gui.add(blendMode.set("Blend mode", 0, 0, 5));
    gui.add(backgroundColor.set("BackgroundColor", ofColor(255), ofColor(0), ofColor(255)));
    gui.add(bSave.set("Save", false));
    
    ofDirectory dir;
    string directory;
    
    auto r = ofSystemLoadDialog("Location of videos", true);
    if ( r.bSuccess ){
        directory = r.getPath();
    } else {
        ofExit();
    }
    
    int n = dir.listDir(directory);
    for ( int i=-1; i<n; i++){
        auto * v = new ofVideoPlayer();
        int ind = i;
        if ( ind < 0 ){
            ind = 0;
        }
        bool l = v->load(dir.getPath(ind));
        if ( l && i >= 0 ){
            videos.push_back(v);
            v->play();
            v->setFrame(0);
            kaleido.addTexture(v);
            triangles.addTexture(v);
            grid.addTexture(v);
            v->setPaused(true);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // update frame counter
//    auto t = 0.f;
//    auto fr = 1000./30.;
    auto f = getTime(true);
    bool bNext = true;
    
//    if ( f - t > fr ){
//        t = f;
//        bNext = true;
//    }
    
    for ( auto * v : videos ){
        v->update();
        if ( bNext ) v->nextFrame();
        if ( v->getCurrentFrame() +1 >= v->getTotalNumFrames() ){
            v->setFrame(0);
        }
    }
    kaleido.update();
    triangles.update();
    grid.update();
    
    static bool bSaving = false;
    if ( bSave && !bSaving ){
        bSaving = true;
        
        auto r = ofSystemSaveDialog("output", "save folder");
        if ( r.bSuccess ){
            saveLocation = r.getPath();
            
        } else {
            bSave.set(false);
        }
        
    } else if (!bSave ){
        bSaving = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableBlendMode((ofBlendMode) blendMode.get() );
    ofBackground(backgroundColor.get());
    if ( drawMode == 0 ){
        kaleido.draw();
    } else if ( drawMode == 1 ){
        grid.draw();
    } else {
        triangles.draw();
    }
    
    if ( bSave ){
        ofSaveScreen(ofToDataPath(saveLocation + "/" + ofGetTimestampString() +".png"));
    }
    
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
