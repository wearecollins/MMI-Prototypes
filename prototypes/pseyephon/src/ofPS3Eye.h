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
        camFrameRate = 60;
        
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
        
        vidGrabber.setDeviceID(idx);
        vidGrabber.setDesiredFrameRate(camFrameRate);
        vidGrabber.setup(camWidth, camHeight);
        
        vidGrabber.setAutogain(false);
        vidGrabber.setAutoWhiteBalance(false);
        tex.allocate(camWidth, camHeight, GL_RGB);
    }
    
    bool update(){
        vidGrabber.update();
        
        vidGrabber.setGain(gain.get());
        vidGrabber.setExposure(exposure.get());
        vidGrabber.setSharpness(sharpness.get());
        vidGrabber.setContrast(contrast.get());
        vidGrabber.setBrightness(brightness.get());
        vidGrabber.setHue(hue.get());
        vidGrabber.setRedBalance(redBalance.get());
        vidGrabber.setBlueBalance(blueBalance.get());
        
        bool bNew = vidGrabber.isFrameNew();
        
        if ( bNew ){
            tex.loadData(vidGrabber.getPixels());
        }
        
        return bNew;
    }
    
    void draw( float x, float y, float w=-1, float h=-1 ){
        if ( w == -1 ) w = camWidth;
        if ( h == -1 ) h = camHeight;
        if ( tex.isAllocated() )
            tex.draw(x,y,w,h);
    }
    
    const ofxPS3EyeGrabber & getCamera() {
        return vidGrabber;
    }
    
    void close(){
        vidGrabber.close();
    }
    
    const ofTexture & getTexture(){
        return tex;
    }
    
    ofParameterGroup params;
    
protected:
    ofxPS3EyeGrabber vidGrabber;
    ofTexture tex;
    
    ofParameter<float> gain, exposure, sharpness, contrast, brightness, hue, redBalance, blueBalance;
    
    int camWidth;
    int camHeight;
    int camFrameRate;
};
