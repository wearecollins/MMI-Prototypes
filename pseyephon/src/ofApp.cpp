#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 320;
    camHeight = 240;
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
            cameras.back()->setDimensions(camWidth,camHeight);
            cameras.back()->setup(i, "PS3 Eye "+ofToString(i));
            panel.add( cameras.back()->params );
        }
        
        ofLogNotice("ofApp::setup") << ss.str();
    }
    
    panel.add(bUseBw.setup("BW filter", true));
    panel.add(bUseChroma.setup("Chroma Key filter", true));
    panel.add(bUpdateColor.setup("Update BG Color", true));
    
    // add chroma to gui
    bg_image.load("background.jpg");
    chromakey = new ofxChromaKeyShader(640, 480);// * ( (int) devices.size()));
    panel.add(chromakey->generalParams);
    panel.add(chromakey->positionParams);
    
    // setup textures
    videoTexture.allocate(640, 480);//camWidth, camHeight * ( (int) devices.size()) );
    
    mainOutputSyphonServer.setName("Screen Output");
    
    effect.load("bw");
    
    panel.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    videoTexture.begin();
    ofClear(0,255);
    int y = 0;
    
    bool bNewFrame = false;
    for ( auto * c : cameras ){
        bool b = c->update();
        if ( b || true ){
            if(bUseBw){
                effect.begin();
                effect.setUniformTexture("tex", c->getTexture(), 1);
            }
            bNewFrame = true;
            ofPushMatrix();
            ofTranslate(camWidth/2.0,0);
            ofScale(-1, 1);
            ofTranslate(-camWidth/2.0,0);
            c->draw(0,y);
            ofPopMatrix();
            if(bUseBw){
                effect.end();
            }
        }
        y += camHeight;
    }
    videoTexture.end();
    
    if (bNewFrame)
    {
        if ( bUseChroma ){
            if ( bUpdateColor){
                bUpdateColor = false;
                static ofPixels readMe;
                videoTexture.readToPixels(readMe);
                chromakey->updateBgColor(readMe);
            }
        
            chromakey->updateChromakeyMask(videoTexture.getTexture(), bg_image.getTexture());
        }
        if ( videoTexture.isAllocated() )
            if (bUseChroma)
                mainOutputSyphonServer.publishTexture(&(chromakey->getFinalImage().getTexture()));
            else
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
    chromakey->getFinalImage().draw(0,0);
    if ( ofGetKeyPressed('c') || !bUseChroma) {
        videoTexture.draw(0, 0);
    }
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
    else {
        effect.unload();
        effect.load("bw");
    }
}
