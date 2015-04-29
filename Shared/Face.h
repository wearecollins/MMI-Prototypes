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

static float EYE_WIDTH = 60.;
static float NOSE_SCALE = 1.0;

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
    
    Face(){
        render.load("", "render.frag");
        bReload = false;
        bActive = false;
        lastActive = -1;
        eyeMult = ofRandom(.2,.6);
        noseType = (NoseType) floor( ofRandom(POINTED + 1));
        if ( noseManager == NULL){
            noseManager = new NoseManager();
            noseManager->setup();
        }
        alpha = 0.0;
    }
    
    float alpha;
    
    void draw(){
        if ( bReload ){
            render.unload();
            render.load("", "render.frag");
            bReload= false;
        }
        if ( !fbo.isAllocated() ){
            fbo.allocate(this->width, this->height, GL_RGBA);
            drawFbo.allocate(this->width, this->height, GL_RGBA);
            yTrans = -height * 1.5;
            eyeOffset = ofRandom(-height/4.0, height/10.0);
            noseScale.set(ofRandom(.5,1.5),ofRandom(.5,1.5));
            noseScale *= NOSE_SCALE;
        }
        
        if ( bActive ){
            yTrans = yTrans * .9 + eyeOffset * .1;
            if ( lastActive == -1 ){
                lastActive = ofGetElapsedTimeMillis();
                duration = ofRandom(500, 3000);
            }
            alpha = alpha * .9 + .1;
        } else {
            alpha = alpha * .9;
            lastActive = -1;
            yTrans = yTrans * .9 + (-height * 1.5) * .1;
        }
        
        if ( ofGetElapsedTimeMillis() - lastActive > duration ){
            bActive = false;
        }
        
        fbo.begin();
        ofClear(0,0);
        ofPushStyle();
        ofSetColor(255);
        ofPushMatrix();
//        ofTranslate(this->x, this->y);
        ofEllipse(this->width/2.0,this->height/2.0, this->width, this->height);
        ofPopMatrix();
        ofPopStyle();
        fbo.end();
        
        drawFbo.begin();
        ofClear(0,0);
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(color);
        ofTranslate(this->width/2.0,this->height/2.0);
        ofEllipse(0,0, width, height);
        //float s = sin( ofGetElapsedTimeMillis() * .001 + (color.r + color.g + color.b));
        drawEye(-EYE_WIDTH * .6, yTrans, EYE_WIDTH, true);
        drawEye(EYE_WIDTH * .6, yTrans, EYE_WIDTH, false);
        ofColor dc = ofColor(color);
        dc.setBrightness(dc.getBrightness() * .8);
        ofSetColor(dc);
        
        noseManager->getNose(noseType).setFillColor(dc);
        // noses are 40px right now
//        ofTranslate(-20,0 * noseScale.x );
        ofScale(noseScale.x, noseScale.x);
        
        noseManager->getNose(noseType).draw(0, yTrans * 1./noseScale.x);
        
//        switch (noseType) {
//            case CIRCLE:
//                noseManager->
//                break;
//                
//            case POINTED:
//                ofTriangle(0, yTrans, 0, yTrans + EYE_WIDTH * noseScale.x, -noseScale.x, yTrans + noseScale.y);
//                ofTriangle(0, yTrans, 0, yTrans + EYE_WIDTH * noseScale.x, noseScale.x, yTrans + noseScale.y);
//                break;
//                
//            case TEARDROP:
//                
//                break;
//        }
        
        ofPopMatrix();
        ofPopStyle();
        drawFbo.end();
        
        render.begin();
        render.setUniform1f("alpha", alpha);
        render.setUniformTexture("mask", fbo.getTextureReference(), 1);
        render.setUniformTexture("draw", drawFbo.getTextureReference(), 2);
        drawFbo.draw(this->x, this->y);
        render.end();
    }
    
    ofColor color;
    bool bReload;
    bool bActive;
    
protected:
    void drawEye( float x, float y, float w, bool bleft = false, float scale = 1.0 ){
        ofSetColor(255);
        ofEllipse(x,y,w,w * scale);
        ofSetColor(0);
        float mult = bleft ? 20. : -20.;
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

