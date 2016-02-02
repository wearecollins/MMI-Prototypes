#include "ofApp.h"

using namespace mmi;

bool bGuiVisible = false;
bool bAddFront = false;

float cam_scale_factor = .5;
float cam_width = 640.;
float cam_height = 480;

mmi::Mode lastMode = MODE_SELECT;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
    
    ofBackground(0);
    
    recorder.setup();
    ofAddListener(recorder.donePlaying, this, &ofApp::donePlayback);
    
    // create GUI
	gui.setup();
    
    // mngr
    auto & mgr = AppManager::get();
    
    // setup screens
    screens[MODE_INTRO] = new IntroScreen();
    screens[MODE_PREVIEW_1] = new PreviewScreen();
    screens[MODE_PREVIEW_2] = new PreviewScreen();
    screens[MODE_SELECT] = new ClipScreen();
    //            screens[MODE_GETREADY] = new PerformScreen();
    //            screens[MODE_PERFORM] = screens[MODE_GETREADY];
    //            screens[MODE_WATCH] = new PreviewScreen();
    //            screens[MODE_SHARE] = new ShareScreen();
    
    for ( auto & it : screens ){
        it.second->setup(it.first);
    }
    
    
    // gui
	gui.add(mgr.params);
	gui.add(doRecord.set("Do recording", true));
    
    // camers
    ofxPS3EyeGrabber dummy;
    std::vector<ofVideoDevice> devices = dummy.listDevices();
    
    if ( devices.size() > 0 && devices[0].bAvailable ){
        cameraLeft = new CameraPS3();
        cameraLeft->setup(0, "Camera Left");
        gui.add(cameraLeft->params);
    }
    
    if ( devices.size() > 1 && devices[1].bAvailable ){
        cameraRight = new CameraPS3();
        cameraRight->setup(1, "Camera Right");
        gui.add(cameraRight->params);
    }
    
    // mode-specific stuff
    auto * intro = (dynamic_cast<IntroScreen*>(screens[MODE_INTRO]));
    intro->loadSound("sound/Pv01.mp3");
    
    auto & event = intro->button.onReleaseEvent;
    ofAddListener(event, this, &ofApp::nextMode);
    
    auto * preview1 = dynamic_cast<PreviewScreen*>(screens[MODE_PREVIEW_1]);
    auto * preview2 = dynamic_cast<PreviewScreen*>(screens[MODE_PREVIEW_2]);
    
    preview1->setTexture( cameraLeft->getTexture() );
    preview1->loadSound("sound/Pv02.mp3");
    preview2->setTexture( cameraRight->getTexture() );
    preview2->loadSound("sound/Pv03.mp3");
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
    auto & mgr = AppManager::get();
    mgr.update();
    
    for ( auto & it : screens ){
        it.second->update();
    }
    
	cameraLeft->update();
	cameraRight->update();
    
    // to-do: record!
    
    if ( mgr.getMode() == MODE_SELECT && lastMode != MODE_SELECT ){
        recorder.clear();
    }
    
    // maybe this should be an event?
    lastMode = mgr.getMode();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    auto & mgr = AppManager::get();
    
    if ( screens.count(mgr.getMode()) != 0)
        screens[mgr.getMode()]->draw();
    
    ofSetColor(255);
    
    if ( bGuiVisible ){
		cameraRight->draw(640, 0);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::nextMode(){
    // to-do: settings fer skippin' modes
    // and and this should be in app mngr may-b
    
    auto & mgr = AppManager::get();
    auto mode = mgr.getMode();
    
    auto lastMode = mode;
    
    if ( (int) mode + 1 < MODE_SHARE ){
        mode = (Mode)( (int) mode + 1);
    }
    if ( screens.count(lastMode) != 0){
        screens[lastMode]->onEnd();
    }
    
    mgr.setMode(mode);
    
    if ( screens.count(mode) != 0){
        screens[mode]->onStart();
    }
}

//--------------------------------------------------------------
void ofApp::donePlayback(){
    auto & mgr = AppManager::get();
    mgr.setMode(MODE_SHARE);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        //process_occlusion =  !process_occlusion;
    } else if (key == 'f') {
        ofToggleFullscreen();
    } else if ( key == 'g' ){
        bGuiVisible = !bGuiVisible;
    } else if ( key == 's' ){
		gui.saveToFile ("settings.xml");
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
