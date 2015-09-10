//
//  RecManager.h
//  EW_Performance
//
//  Created by Brett Renfer on 7/9/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"

class RecManager {
public:
    
    void setup(){
        rate    = 16.67;
        index   = 0;
		bNeedToRender = false;
        bumper.load("images/mmi_bumper.png");
        bumperDur = 1000;
    }
    
    void update(){
        if ( index == -1 ){
            if ( ofGetElapsedTimeMillis() - lastChanged > bumperDur ){
                nextFrame();
            }
        } else if ( ofGetElapsedTimeMillis() - lastChanged > rate ){
            nextFrame();
        }
    }
    
    void draw(){
        if ( !bDrawing ) return;
        update();
		static ofImage image;
        if ( index == -1){
            bumper.draw(0,0);
        } else {
            if (bNeedToRender) {
                image.clear();
                image.setFromPixels(pixels[index]);
                image.update();
                bNeedToRender = false;
            }
            image.draw(0,0);
        }
    }
    
    void addFrame( ofPixels & pix ){
		pixels.push_back(pix);
		bNeedToRender = true;
    }
    
    void clear(){
		pixels.clear();
		bNeedToRender = true;
        endRender();
    }
    
    void beginRender(){
        if ( bDrawing ) return;
        bDrawing = true;
        lastChanged = ofGetElapsedTimeMillis();
        index = -1;
    }
    
    void endRender(){
        bDrawing = false;
        index = -1;
    }
    
    int rate;
    
protected:
    
    void nextFrame(){
        lastChanged = ofGetElapsedTimeMillis();
        index++;
        if ( index >= pixels.size()){
            index = -1;
        }
        bNeedToRender = true;
    }
    
    uint64_t lastChanged;
    int index;
    vector<ofPixels> pixels;
    ofImage bumper;
	bool bNeedToRender, bDrawing;
    int bumperDur;
};
