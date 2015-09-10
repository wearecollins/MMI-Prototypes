#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    camWidth = 640;
    camHeight = 480;
    camFrameRate = 60;
    
    //we can now get back a list of devices.
    std::vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(std::size_t i = 0; i < devices.size(); ++i)
    {
        std::stringstream ss;
        
        ss << devices[i].id << ": " << devices[i].deviceName;
        
        if(!devices[i].bAvailable)
        {
            ss << " - unavailable ";
        }
        
        ofLogNotice("ofApp::setup") << ss.str();
    }
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(camFrameRate);
    vidGrabber.setup(camWidth, camHeight);
    
    vidGrabber.setAutogain(false);
    vidGrabber.setAutoWhiteBalance(false);
    
    
    mainOutputSyphonServer.setName("Screen Output");
    
    panel.setup();
    panel.add(gain.set("Gain", 1., 0., 100.));
    panel.add(exposure.set("exposure", 1., 0., 100.));
    panel.add(sharpness.set("sharpness", 1., 0., 100.));
    panel.add(contrast.set("contrast", 1., 0., 100.));
    panel.add(brightness.set("brightness", 1., 0., 100.));
    panel.add(hue.set("hue", 1., 0., 255.));
    panel.add(redBalance.set("redBalance", 1., 0., 255.));
    panel.add(blueBalance.set("blueBalance", 1., 0., 255.));
    panel.loadFromFile("settings.xml");
    bDrawGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew())
    {
        videoTexture.loadData(vidGrabber.getPixels());
        if ( videoTexture.isAllocated() )
            mainOutputSyphonServer.publishTexture(&videoTexture);
    }
    
    vidGrabber.setGain(gain.get());
    vidGrabber.setExposure(exposure.get());
    vidGrabber.setSharpness(sharpness.get());
    vidGrabber.setContrast(contrast.get());
    vidGrabber.setBrightness(brightness.get());
    vidGrabber.setHue(hue.get());
    vidGrabber.setRedBalance(redBalance.get());
    vidGrabber.setBlueBalance(blueBalance.get());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    
    videoTexture.draw(0, 0);
    
    std::stringstream ss;
    
    ss << "App FPS: " << ofGetFrameRate() << std::endl;
    ss << "Cam FPS: " << vidGrabber.getFPS();
    
    ofDrawBitmapStringHighlight(ss.str(), ofPoint(10, 15));
    if ( bDrawGui ) panel.draw();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    vidGrabber.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ) bDrawGui = !bDrawGui;
}
