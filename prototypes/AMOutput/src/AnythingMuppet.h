//
//  AnythingMuppet.h
//  AMOutput
//
//  Created by Brett Renfer on 4/28/15.
//
//

#pragma once
#include "ofMain.h"
#include "ofxSvg.h"


class AnythingMuppet : public ofNode {
public:
    
    void load( string file ) {
        body.load(file);
        setPosition(ofGetWidth()/2.0 - body.getWidth()/2.0,MAX(50.,ofGetHeight()/2.0 - body.getHeight()/2.0), 0);
    }
    
    void draw(){
        ofTranslate( getPosition() );
        body.draw();
    }
    
    ofColor color;
    
protected:
    ofxSVG body;
};
