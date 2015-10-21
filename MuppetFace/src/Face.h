//
//  Face.h
//  FaceWall
//
//  Created by Brett Renfer on 11/16/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

static float EYE_WIDTH = 15.;
static float NOSE_SCALE = .5;

enum NoseType {
    CIRCLE = 0,
    TEARDROP,
    POINTED
};

class NoseManager {
public:

    void setup(){
        ofxSVG svg;
        svg.load("noses.svg");
        
        cout <<"NOSES "<<svg.getNumPath()<<endl;
        
        noses[CIRCLE] = svg.getPathAt(0);
        noses[TEARDROP] = svg.getPathAt(1);
        noses[POINTED] = svg.getPathAt(2);
    }
    
    ofPath & getNose( NoseType type ){
        return noses[type];
    }
    
protected:
    
    map<NoseType,ofPath> noses;
};

static NoseManager * noseManager = NULL;

class Face : public ofRectangle
{
public:
    
    
    void setup(){
        bReload = false;
        bActive = false;
        lastActive = -1;
        noseType = (NoseType) floor( ofRandom(POINTED + 1));
        if ( noseManager == NULL){
            noseManager = new NoseManager();
            noseManager->setup();
        }
    }
    
    void reset(){
        eyeMult = ofRandom(.2,.9);
        fbo.allocate(this->width, this->height, GL_RGBA);
        drawFbo.allocate(this->width, this->height, GL_RGBA);
        yTrans = -height * 1.5;
        eyeOffset = ofRandom(-height/3.0, height/10.0);
        noseScale.set(ofRandom(.5,1.5),ofRandom(.5,1.5));
        noseScale *= NOSE_SCALE;
    }
    
    void draw(){
        if ( !fbo.isAllocated() ){
            reset();
        }
        
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(color);
        ofTranslate(this->x, this->y);
        ofEllipse(this->width/2.0,this->height/2.0, width, height);
        
        drawEye(eye1, true);
        drawEye(eye2, false);
        ofColor dc = ofColor(color);
        dc.setBrightness(dc.getBrightness() * .8);
        ofSetColor(dc);
        
        float nScale = noseScale.x * width/eye1.width;
        
        noseManager->getNose(noseType).setFillColor(dc);
        // noses are 40px right now
        ofTranslate(this->width/2.0,eye1.y + nScale);
        ofScale(nScale, nScale);
        
        noseManager->getNose(noseType).draw(0, 0);
        
        ofPopMatrix();
        ofPopStyle();
    }
    
    ofColor color;
    bool bReload;
    bool bActive;
    
    ofRectangle eye1, eye2;
    
protected:
    void drawEye( ofRectangle rect, bool bleft = false, float scale = 1.0 ){
        drawEye(rect.x + rect.width/2.0,rect.y + rect.height/2.0,rect.height,bleft, scale);
    }
    void drawEye( float x, float y, float w, bool bleft = false, float scale = 1.0 ){
        ofSetColor(255);
        ofEllipse(x,y,w,w * scale);
        ofSetColor(0);
        float mult = bleft ? 2.0 : -2.0;
        ofEllipse(x + mult * eyeMult,y,w * eyeMult, w * eyeMult * scale);
    }
    
    float yTrans;
    
    ofShader render;
    ofFbo fbo, drawFbo;
    
    float eyeOffset, eyeMult;
    
    //nose
    NoseType noseType;
    ofVec2f noseScale;
    
    // quick timer
    int lastActive, duration;
};

