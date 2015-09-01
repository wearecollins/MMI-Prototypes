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
    }
    
    void update(){
        if ( ofGetElapsedTimeMillis() - lastChanged > rate ){
            lastChanged = ofGetElapsedTimeMillis();
            index++;
            if ( index >= pixels.size()){
                index = 0;
            }
			bNeedToRender = true;
        }
    }
    
    void draw(){
        update();
		static ofImage image;
		if (bNeedToRender) {
			image.clear();
			image.setFromPixels(pixels[index]);
			image.update();
			bNeedToRender = false;
			ofLogError() << "load " << index << endl;
		}
		image.draw(0,0);
    }
    
    void addFrame( ofPixels & pix ){
		pixels.push_back(pix);
		bNeedToRender = true;
    }
    
    void clear(){
        images.clear();
		pixels.clear();
		bNeedToRender = true;
    }
    
    int rate;
    
protected:
    uint64_t lastChanged;
    int index;
    vector<ofPixels> pixels;
	vector<ofImage> images;
	bool bNeedToRender;
};
