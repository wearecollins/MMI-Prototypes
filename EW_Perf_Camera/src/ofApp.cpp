#include "ofApp.h"

ofImage * background;

//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 320;
    camHeight = 240;
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
    mainParams.add(bUsePb.setup("Use pixelbuffer", true));
    mainParams.add(bUseWs.setup("Use WebSocket", false));
    
    panel.add(&mainParams);
    
    setupCameras();
    
    // add chroma to gui
    bg_image.load("background.jpg");
    background = &bg_image;
    chromakey = new ofxChromaKeyShader(camWidth, camHeight);
    
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
    videoTexture.allocate(640, 480);
    
    mainOutputSyphonServer.setName("Screen Output");
    //websocketStream.setup();
    
    if(ofIsGLProgrammableRenderer()){
        effect.load("shaders/bw_150");
    } else {
        effect.load("shaders/bw");
    }
    
    panel.loadFromFile("settings.xml");
    bDrawGui = true;
    
    // to read in QT: ffmpeg -i testMovie2016-01-31-21-58-06-095.mp4 -vf format=yuv420p test02.mp4
}

//--------------------------------------------------------------
void ofApp::update(){
    // draw to stream texture
    int y = 0;
    
    bool bNewFrame = false;
    
    int ind1 = whichCameraTop.get();
    int ind2 = (ind1 == 0) ? 1 : 0;
    if ( cameras.size() < 2 ){
        ind1 = ind2 = 0;
    }
    
    auto * top = cameras[ind1];
    
    // temp hax
    int ind = 0;
    
#ifdef USE_VG
    for ( int i=0; i<2; i++ ){
        auto * c = cameras[0];
#else
    for ( auto * c : cameras ){
#endif
        c->update();
        
        if ( (ind == 0 && ind1 == 0) || ( ind == 1 && ind1 == 1) ){
            cameraTop.begin();
        } else {
            cameraSide.begin();
            
            if(bUseBw){
                effect.begin();
                effect.setUniformTexture("tex", c->getTexture(), 0);
            }
        }
        ofClear(0,255);
        
        ofPushMatrix();
        
        // this should be a var
        c->draw(0,0);
        ofPopMatrix();
        
        if ( ind == 0 ){
            cameraTop.end();
        } else {
            cameraSide.end();
            
            if(bUseBw){
                effect.end();
            }
        }
        ind++;
    }
        
#ifdef USE_VG
    if ( cameras[ind1]->isFrameNew() || cameras[ind2]->isFrameNew() ){
#else
    if ( cameras[ind1]->isFrameNew() || cameras[ind2]->isFrameNew() ){
#endif
        bNewFrame = true;
        
        if ( bUseChroma){
            chromakey->updateChromakeyMask(cameraTop.getTexture(), background->getTexture());
        }
        
        // STREAM
        videoTexture.begin(); {
            float drawWidth = (camWidth == videoTexture.getWidth() ) ? camWidth/2.0 : camWidth;
            float drawHeight = (camHeight == videoTexture.getHeight() ) ? camHeight/2.0 : camHeight;
            ofClear(0,255);
            
            if(bUseBw){
                effect.begin();
            }
            if ( bUseChroma && background->isAllocated()){
                effect.setUniformTexture("tex", chromakey->fbo_final.getTexture(), 0);
                chromakey->drawFinalImage(0,0, drawWidth, drawHeight);
            } else {
                effect.setUniformTexture("tex", cameraTop.getTexture(), 0);
                cameraTop.draw(0,0, drawWidth, drawHeight);
            }
            if(bUseBw){
                effect.end();
            }
            cameraSide.draw(0,drawHeight, drawWidth, drawHeight);
        } videoTexture.end();
        
        static ofPixels pixelsTop;
        static ofPixels pixelsBottom;
        
        if (bUsePb){
             for (int i=0; i<2; i++){
                 if ( i == 0 ){
                     if (bUseChroma){
                         chromakey->fbo_final.getTexture().copyTo(pixelBufferBack[i]);
                     } else {
                         cameraTop.getTexture().copyTo(pixelBufferBack[i]);
                     }
                 } else {
                    cameraSide.getTexture().copyTo(pixelBufferBack[i]);
                 }
                 pixelBufferFront[i].bind(GL_PIXEL_UNPACK_BUFFER);
                 unsigned char * p = pixelBufferFront[i].map<unsigned char>(GL_READ_ONLY);
                 pixelBufferFront[i].unbind(GL_PIXEL_UNPACK_BUFFER);
                 if ( i == 0 ){
                     pixelsTop.setFromExternalPixels(p,cameraTop.getWidth(),cameraTop.getHeight(),OF_PIXELS_RGBA);
                 } else {
                     pixelsBottom.setFromExternalPixels(p,cameraSide.getWidth(),cameraSide.getHeight(),OF_PIXELS_RGBA);
                 }
                 pixelBufferFront[i].bind(GL_PIXEL_UNPACK_BUFFER);
                 pixelBufferFront[i].unmap();
                 pixelBufferFront[i].unbind(GL_PIXEL_UNPACK_BUFFER);
                 swap(pixelBufferBack[i],pixelBufferFront[i]);
//                 delete p;
            }
        } else {
            if ( bUseChroma ){
//                pixelsTop = chromakey->getFinalImage().getPixels();
                chromakey->fbo_final.readToPixels(pixelsTop);
                
                // need to b-w post chromakey
                if ( bUseBw ){
                    pixelsTop.setImageType(OF_IMAGE_GRAYSCALE);
                }
            } else {
                cameraTop.readToPixels(pixelsTop);
            }
            cameraSide.readToPixels(pixelsBottom);
        }
        
        if ( bUpdateColor ){
            bUpdateColor = false;
            chromakey->updateBgColor( pixelsTop );
        }
        
        recorder.update(pixelsTop, pixelsBottom);
    }

        
    // SYPHON
    if (bNewFrame) {
        if ( videoTexture.isAllocated() && !bUseWs){
            mainOutputSyphonServer.publishTexture(&(videoTexture.getTexture()));
        } else if ( videoTexture.isAllocated() && bUseWs ){
            websocketStream.streamFbo(videoTexture);
        } else {
        }
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
void ofApp::setupCameras(){
#ifndef USE_VG
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
#else//we can now get back a list of devices.
    
    cameras.push_back( new ofVideoGrabber() );
    cameras.back()->setDeviceID(2);
    cameras.back()->setDesiredFrameRate(camFrameRate);
    cameras.back()->setup(camWidth,camHeight);
#endif
    
    cameraTop.allocate(camWidth, camHeight);
    cameraSide.allocate(camWidth, camHeight);
    
    pixelBufferBack[0].allocate(camWidth*camHeight*4,GL_DYNAMIC_READ);
    pixelBufferFront[0].allocate(camWidth*camHeight*4,GL_DYNAMIC_READ);
    pixelBufferBack[1].allocate(camWidth*camHeight*4,GL_DYNAMIC_READ);
    pixelBufferFront[1].allocate(camWidth*camHeight*4,GL_DYNAMIC_READ);
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
        
        if(ofIsGLProgrammableRenderer()){
            effect.load("shaders/bw_150");
        } else {
            effect.load("shaders/bw");
        }
    }
    
    // for now
    if ( key == 'r' ){
        recorder.startRecording( clips["horse_named_bill"].getClip() );
    }
}

//--------------------------------------------------------------
void ofApp::setupSpacebrew(){
    spacebrew.addPublish("recording", Spacebrew::TYPE_STRING);
    spacebrew.addSubscribe("startRecording", "string");
    spacebrew.addSubscribe("getReady", "string");
    
    spacebrew.connect("spacebrew.robotconscience.com", Spacebrew::SPACEBREW_PORT);
    
    ofAddListener( spacebrew.onMessageEvent, this, &ofApp::onMessage );
}

//--------------------------------------------------------------
void ofApp::onMessage( Spacebrew::Message & message ){
    if (message.name == "startRecording" ){
//        static Json::Reader jsonParser;
//        static Json::Value json;
//        if( jsonParser.parse(message.value, json) ){
//            /*****
//             data:
//             {
//                name:"clip name"
//             }
//             // other stuff
//             ****/
//            string clipName = json["name"].asString();
//            if ( clips.count(clipName) != 0 ){
//                
//                recorder.startRecording( clips[clipName].getClip() );
//            }
//        }
        
        if ( clips.count(message.valueString()) != 0 ){
            background = &clips[message.valueString()].background;
            recorder.startRecording( clips[message.valueString()].getClip() );
        }
    } else if (message.name == "getReady" ){
        
        if ( clips.count(message.valueString()) != 0 ){
            background = &clips[message.valueString()].background;
        }
    }
}

//--------------------------------------------------------------
void ofApp::onRecordComplete( string & complete ){
    // ooooook
    string cmd = "mv "+ofToDataPath(complete, true)+" ../../../../../_web/MMI_Performance/v3/public/uploads/" + complete;
    
    system(cmd.c_str() );
    
    spacebrew.sendString("recording", "uploads/" + complete);
    background = &bg_image;
}