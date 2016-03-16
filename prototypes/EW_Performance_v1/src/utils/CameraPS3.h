//
//  CameraPS3.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "CameraManager.h"

#include "ofxPS3EyeGrabber.h"

namespace mmi {
    class CameraPS3 : public Camera {
    public:
        
        CameraPS3() : Camera()
        {
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
        
        
        ~CameraPS3(){
            close();
        }
        
        // set me up!
        void setup( int idx = 0, string name = "PS3 Eye 1"){
            Camera::setup(idx, name);
            
            cout << "SETTING UP "<<idx<<endl;
            
            vidGrabber.setDeviceID(idx);
            vidGrabber.setDesiredFrameRate(camFrameRate);
            vidGrabber.setup(camWidth, camHeight);
            
            vidGrabber.setAutogain(false);
            vidGrabber.setAutoWhiteBalance(false);
            tex.allocate(camWidth, camHeight, GL_RGB);
        }
        
        void update(){
            Camera::update();
            
            vidGrabber.update();
            
            vidGrabber.setGain(gain.get());
            vidGrabber.setExposure(exposure.get());
            vidGrabber.setSharpness(sharpness.get());
            vidGrabber.setContrast(contrast.get());
            vidGrabber.setBrightness(brightness.get());
            vidGrabber.setHue(hue.get());
            vidGrabber.setRedBalance(redBalance.get());
            vidGrabber.setBlueBalance(blueBalance.get());
            
            bIsFrameNew = vidGrabber.isFrameNew();
            
            if ( bIsFrameNew ){
                tex.loadData(vidGrabber.getPixels());
            }
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
        
    protected:
        ofxPS3EyeGrabber vidGrabber;
        
        ofParameter<float> gain, exposure, sharpness, contrast, brightness, hue, redBalance, blueBalance;
    };
}
