//
//  Script.h
//  AM_Scripts_01
//
//  Created by Brett Renfer on 1/20/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxEasingFunc.h"


enum Mode {
    MODE_STACK = 0,
    MODE_FAN,
    MODE_SIDE_BY
};

class Script : public ofRectangle {
public:
    
    void setup(float w, float h){
        color.set(255);
        color.setSaturation(150);
        color.setBrightness(150);
        color.setHue( ofRandom(255) );
        acc.set(.9,.9);
        vel.set(0);
        
        this->width = w;
        this->height = h;
        
        bPressed = false;
        ofSetRectMode(OF_RECTMODE_CENTER);
    }
    
    void update(){
        this->x += vel.x;
        this->y += vel.y;
        
        vel *= acc;
    }
    
    void draw(){
        ofPushStyle();
        ofSetColor(this->color);
        ofPushMatrix();
        ofTranslate(this->x,this->y);
        ofRotateZ(this->rotation);
        ofDrawRectangle(0,0, this->width, this->height);
        ofPopMatrix();
        ofPopStyle();
    }
    
    bool mousePressed( int x, int y ){
        bPressed = bInside(x,y);
        lastMouse.set(x,y);
        return bPressed;
    }
    
    bool mouseMoved( int x, int y, Mode m ){
        if (!bPressed) return;
        else {
            switch (m){
                case MODE_FAN:
                    this->x -= (lastMouse.x - x);
                    this->y -= (lastMouse.y - y);
                    lastMouse.set(x,y);
                    break;
            }
        }
    }
    
    bool mouseReleased(int x, int y ){
        bPressed = false;
    }
    
    float rotation;
    ofColor color;
    ofVec2f vel, acc;
    
protected:
    
    bool bPressed;
    ofVec2f lastMouse;
    
    bool bInside( int x, int y ){
        ofVec2f start(ofGetWidth()/2.0, ofGetHeight()/2.0);
        ofVec2f minDim(this->x-width/2.0, this->y-height/2.0);
        ofVec2f maxDim(this->x+width/2.0, this->y+height/2.0);
        
        if ( x < (start.x + maxDim.x ) && x > start.x + minDim.x &&
            y < start.y + maxDim.y && y > start.y + minDim.y ){
            return true;
        }
        
        return false;
    }
};