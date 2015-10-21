#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 640;
    camHeight = 480;
    camFrameRate = 30;
    
    ofSetFrameRate(camFrameRate);
    ofSetVerticalSync(true);
    
    // start gui
    
    panel.setup();
    
    //we can now get back a list of devices.
    ofxPS3EyeGrabber dummy;
    
    std::vector<ofVideoDevice> devices = dummy.listDevices();
    
    for(std::size_t i = 0; i < devices.size(); ++i)
    {
        std::stringstream ss;
        
        ss << devices[i].id << ": " << devices[i].deviceName;
        
        if(!devices[i].bAvailable)
        {
            ss << " - unavailable ";
        } else {
            cameras.push_back( new ofPS3Eye() );
            cameras.back()->setFrameRate(camFrameRate);
            cameras.back()->setup(i, "PS3 Eye "+ofToString(i));
            panel.add( cameras.back()->params );
        }
        
        ofLogNotice("ofApp::setup") << ss.str();
    }
    
    videoTexture.allocate(camWidth, camHeight * ( (int) devices.size()) );
    
    mainOutputSyphonServer.setName("Screen Output");
    
    
    panel.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    videoTexture.begin();
    int y = 0;
    
    bool bNewFrame = false;
    for ( auto * c : cameras ){
        bool b = c->update();
        if ( b || true ){
            bNewFrame = true;
            c->draw(0,y);
        }
        y += camHeight;
    }
    videoTexture.end();
    
    if (bNewFrame)
    {
        if ( videoTexture.isAllocated() )
            mainOutputSyphonServer.publishTexture(&(videoTexture.getTexture()));
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    
    float s = MIN( ofGetWidth() / videoTexture.getWidth(), ofGetHeight() / videoTexture.getHeight());
    
    ofPushMatrix();
    ofScale(s, s);
    videoTexture.draw(0, 0);
    ofPopMatrix();
    
    std::stringstream ss;
    
    ss << "App FPS: " << ofGetFrameRate() << std::endl;
    
    ofDrawBitmapStringHighlight(ss.str(), ofPoint(10, 15));
    if ( bDrawGui ) panel.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    for ( auto * c : cameras ){
        c->close();
        delete c;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ) bDrawGui = !bDrawGui;
}
