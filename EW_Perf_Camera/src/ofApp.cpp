#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 640;
    camHeight = 480;
    camFrameRate = 30;
    
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    
    // start gui
    
    panel.setup();
    
    mainParams.setup("Main parameters");
    mainParams.add(bUseBw.setup("BW filter", true));
    mainParams.add(bUseChroma.setup("Chroma Key filter", true));
    mainParams.add(bUpdateColor.setup("Update BG Color", true));
    mainParams.add(whichCameraTop.set("Which cam is top", 0, 0, 1));
    
    panel.add(&mainParams);
    
    setupCameras();
    
    // add chroma to gui
    bg_image.load("background.jpg");
    chromakey = new ofxChromaKeyShader(640, 480);
    
    panel.add(chromakey->generalParams);
    panel.add(chromakey->positionParams);
    
    // setup recorder
    recorder.setup();
    
    ofAddListener( recorder.onFinishedRecording , this, &ofApp::onRecordComplete );
    
    panel.add(recorder.params);
    
    // setup backgrounds
    clips["black_magic"].setup("black_magic");
    clips["cest_bon"].setup("cest_bon");
    clips["horse_named_bill"].setup("horse_named_bill");
    
    // setup spacebrew
    setupSpacebrew();
    
    // setup textures
    videoTexture.allocate(camWidth, camHeight);
    
    mainOutputSyphonServer.setName("Screen Output");
    
    effect.load("shaders/bw");
    
    panel.loadFromFile("settings.xml");
    bDrawGui = true;
    
    
    // to read in QT: ffmpeg -i testMovie2016-01-31-21-58-06-095.mp4 -vf format=yuv420p test02.mp4
}

//--------------------------------------------------------------
void ofApp::update(){
    // draw to stream texture
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
            
            // this should be a var
            c->draw(0,y,camWidth/2.0, camHeight/2.0);
            ofPopMatrix();
            if(bUseBw){
                effect.end();
            }
        }
        y += camHeight/2.0;
    }
    videoTexture.end();
    
    static ofPixels readMe;
    static bool bRead = false;
    
    if (bNewFrame) {
        bRead = false;
        
        if ( bUseChroma ){
            if ( bUpdateColor ){
                bUpdateColor = false;
                auto & pix = cameras[whichCameraTop.get()]->getCamera().getPixels();
                chromakey->updateBgColor( pix );
            }
        
            chromakey->updateChromakeyMask(videoTexture.getTexture(), bg_image.getTexture());
        }
        if ( videoTexture.isAllocated() ){
            if (bUseChroma){
                mainOutputSyphonServer.publishTexture(&(chromakey->getFinalImage().getTexture()));
            } else {
                mainOutputSyphonServer.publishTexture(&(videoTexture.getTexture()));
            }
        }
        
        int ind1 = whichCameraTop.get();
        int ind2 = (ind1 == 0) ? 1 : 0;
        
        recorder.update(cameras[ind1]->getCamera().getPixels(), cameras[ind2]->getCamera().getPixels());
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
void ofApp::setupCameras(){
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
}

//--------------------------------------------------------------
void ofApp::exit()
{
    for ( auto * c : cameras ){
        c->close();
        delete c;
    }
    
    recorder.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ) bDrawGui = !bDrawGui;
    else if ( key == 'R' ){
        effect.unload();
        effect.load("shaders/bw");
    }
    
    // for now
    if ( key == 'r' ){
        recorder.startRecording( clips["horse_named_bill"].getClip() );
    }
}

//--------------------------------------------------------------
void ofApp::setupSpacebrew(){
    spacebrew.addPublish("recording", Spacebrew::TYPE_STRING);
    spacebrew.addSubscribe("startRecording", "object");
    spacebrew.connect(Spacebrew::SPACEBREW_CLOUD, Spacebrew::SPACEBREW_PORT);
    
    ofAddListener( spacebrew.onMessageEvent, this, &ofApp::onMessage );
}

//--------------------------------------------------------------
void ofApp::onMessage( Spacebrew::Message & message ){
    if (message.name == "startRecording" ){
        static Json::Reader jsonParser;
        static Json::Value json;
        if( jsonParser.parse(message.value, json) ){
            /*****
             data:
             {
                name:"clip name"
             }
             // other stuff
             ****/
            string clipName = json["name"].asString();
            if ( clips.count(clipName) != 0 ){
                
                recorder.startRecording( clips[clipName].getClip() );
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::onRecordComplete( string & complete ){
    spacebrew.sendString("recording", complete);
}