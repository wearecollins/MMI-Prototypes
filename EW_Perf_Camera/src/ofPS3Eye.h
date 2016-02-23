//
//  ofPS3Eye
//  pseyephon
//
//  Created by Brett Renfer on 10/19/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxPS3EyeGrabber.h"

class ofPS3Eye {
public:
    
    ofPS3Eye(){
        camWidth = 640;
        camHeight = 480;
        camFrameRate = 30;
        
        params.add(gain.set("Gain", 1., 0., 100.));
        params.add(exposure.set("exposure", 1., 0., 100.));
        params.add(sharpness.set("sharpness", 1., 0., 100.));
        params.add(contrast.set("contrast", 1., 0., 100.));
        params.add(brightness.set("brightness", 1., 0., 100.));
        params.add(hue.set("hue", 1., 0., 255.));
        params.add(redBalance.set("redBalance", 1., 0., 255.));
        params.add(blueBalance.set("blueBalance", 1., 0., 255.));
    }
    
    // call before setup
    void setDimensions( int w, int h ){
        camWidth = w;
        camHeight = h;
    }
    
    void setFrameRate( int fr ){
        camFrameRate = fr;
    }
    
    // set me up!
    void setup( int idx = 0, string name = "PS3 Eye 1"){
        params.setName(name);
        
        vidGrabber = std::make_shared<ofVideoGrabber>();
        vidGrabber->setGrabber(std::make_shared<ofxPS3EyeGrabber>());
        
        vidGrabber->setDeviceID(idx);
        vidGrabber->setDesiredFrameRate(camFrameRate);
        
        vidGrabber->setPixelFormat(OF_PIXELS_RGBA);
        vidGrabber->setup(camWidth, camHeight);

        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setHorizontalFlip(true);
//        vidGrabber->setAutogain(false);
//        vidGrabber->setAutoWhiteBalance(false);
        tex.allocate(camWidth, camHeight, GL_RGBA);
    }
    
    bool update(){
        vidGrabber->update();
        
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setGain(gain.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setExposure(exposure.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setSharpness(sharpness.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setContrast(contrast.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setBrightness(brightness.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setHue(hue.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setRedBalance(redBalance.get());
        vidGrabber->getGrabber<ofxPS3EyeGrabber>()->setBlueBalance(blueBalance.get());
        
        bool bNew = vidGrabber->isFrameNew();
        
        if ( bNew ){
//            tex.loadData(vidGrabber->getPixels());
        }
        
        return bNew;
    }
    
    void draw( float x, float y, float w=-1, float h=-1 ){
        if ( w == -1 ) w = camWidth;
        if ( h == -1 ) h = camHeight;
//        if ( tex.isAllocated() )
//            tex.draw(x,y,w,h);
        vidGrabber->draw(x,y,w,h);
    }
    
    bool isFrameNew() {
        return vidGrabber->isFrameNew();
    }
    
    void close(){
        vidGrabber->close();
    }
    
    const ofTexture & getTexture(){
        return tex;
    }
    
    ofParameterGroup params;
    
protected:
    std::shared_ptr<ofVideoGrabber> vidGrabber;
//    ofxPS3EyeGrabber vidGrabber;
    ofTexture tex;
    
    ofParameter<float> gain, exposure, sharpness, contrast, brightness, hue, redBalance, blueBalance;
    
    int camWidth;
    int camHeight;
    int camFrameRate;
};
