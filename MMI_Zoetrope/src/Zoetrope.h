//
//  Zoetrope
//  MMI_Zoetrope
//
//  Created by Brett Renfer on 10/8/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxImageSequence.h"

static float which = 0;

class Zoetrope : public ofRectangle {
public:
    
    bool load( string directory ){
//        sequence.enableThreadedLoad(true);
        sequence.setMaxFrames(100);
        loaded = sequence.loadSequence( directory );
        if ( loaded ){
            sequence.preloadAllFrames();
            this->width  = ofGetWidth();
            this->height = sequence.getHeight();
        }
        activeIndex = 0;
        fIndex = 0;
        activeWidth = ofRandom(3,5);
        lastChanged = lastAnimated = 0;
        changeTime = ofRandom(100,500);
        
        dir = ofRandomuf() > .5 ? 0 : 1;
        
        noiseOffset = which;
        which += ofRandom(100);
        bGrowing = round(ofRandomuf());
        
        if ( bGrowing ){
            changeTime = ofRandom(5000,10000);
        } else {
            changeTime = ofRandom(10000,30000);
        }
        
        return loaded;
    }
    
    void draw( float s ){
        if (!loaded) return;
        
        float clipScale = this->height / sequence.getHeight();
        float cH = sequence.getHeight() * clipScale;
        float cW = sequence.getWidth() * clipScale;
        
        this->width  = ofGetWidth() * 1./s;
        int nClips = ceil(this->width/(cW)) + 1; // this should be < sequence #...
        
        float dx = dir == 0 ? this->x + (cW * (nClips - 2)) : this->x;
        float dy = this->y;
        
        ofPushStyle();
        
        int skip = 2;
        auto t = ofGetElapsedTimeMillis();
        
        for (int i=0; i<nClips; i++){
            float cur = ofMap(t - lastChanged, 0, changeTime, 0., 1.);
            float a = 50 + ofNoise( i + ofGetElapsedTimef() * .5f + noiseOffset) * 255.;
//            if ( i < activeIndex ){
//                float t = ofMap(i, activeIndex - activeWidth, activeIndex, 100, 255, true);
//                float b = ofMap(i+1, activeIndex - activeWidth, activeIndex, 100, 255, true);
//                a = ofMap(cur, 0., 1., t, b);
//            } else {
//                float t = ofMap(i, activeIndex, activeIndex + activeWidth, 255, 100, true);
//                float b = ofMap(i+1, activeIndex, activeIndex + activeWidth, 255, 255, true);
//                a = ofMap(cur, 0., 1., t, b);
//            }
            ofSetColor(255, a);
            
            sequence.getTextureForFrame(fIndex + (i*skip) ).draw(dx,dy, cW, cH);
            dx += cW * ( dir == 0 ? -1 : 1);
        }
        
        
        // change active frame
        if ( t - lastChanged >= changeTime ){
            lastChanged = t;
            activeIndex += (dir == 0 ? -1 : 1);
            if ( activeIndex > nClips + activeWidth ){
                activeIndex = 0;
                changeTime = ofRandom(100,500);
                activeWidth = ofRandom(3,5);
            } else if ( activeIndex < 0 ){
                activeIndex = nClips + activeWidth;
            }
        }
        
//        if ( t - lastChanged >= changeTime ){
//            lastChanged = t;
////            changeTime = ofRandom(500,2000);
//            bGrowing = !bGrowing;
//            if ( bGrowing ){
//                changeTime = ofRandom(5000,10000);
//            } else {
//                changeTime = ofRandom(10000,30000);
//            }
//        }
//        
//        if ( bGrowing ){
//            this->height = ofMap(t - lastChanged, 0, 1000, originalHeight, originalHeight * 2.0, true);
//        } else {
//            this->height = ofMap(t - lastChanged, 0, 1000, originalHeight * 2.0, originalHeight, true);
//        }
        
        // "animate"
        static float animateRate = 1000./10.;
        
        if ( t - lastAnimated >= animateRate ){
            lastAnimated = t;
            fIndex++;
            if ( fIndex > sequence.getTotalFrames()){
                fIndex = 0;
            }
        }
        
        ofPopStyle();
    }
    
    ofParameter<ofVec2f>    position;
    ofParameter<float>      drawHeight;
    
    void setHeight( float h ){
        this->height = h;
        this->originalHeight = h;
    }
    
protected:
    
    ofxImageSequence sequence;
    int activeIndex, activeWidth, fIndex;
    uint64_t lastChanged, lastAnimated;
    float changeTime;
    bool loaded;
    int dir;
    
    float noiseOffset;
    
    // oscillating height
    float originalHeight;
    bool bGrowing;
};
