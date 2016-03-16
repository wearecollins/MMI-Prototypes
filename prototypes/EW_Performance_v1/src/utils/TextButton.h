//
//  Button.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "ofxMSAInteractiveObject.h"
#include "Constants.h"

class TextButton : public ofxMSAInteractiveObject {
public:
    
    TextButton() :
    ofxMSAInteractiveObject()
    {
        color.set(ofColor(150,0,0));
        colorActive.set(ofColor(255,0,0));
        
        fontColor.set(ofColor(255));
        myText.set("Touch");
        padding.set(20);
        font = nullptr;
        this->x = 0;
        this->y = 0;
        autoCenter.set(true);
        
        disableAppEvents();
        enableMouseEvents();
    }
    
    void draw(){
        // update based on text
        if ( font  != nullptr ){
            auto sw = font->stringWidth(myText);
            auto sh = font->stringHeight( myText );
            this->width = sw + padding * 2;
            this->height = sh + padding * 2;
            
            if ( autoCenter ){
                this->x = ofGetWidth()/2.0 - this->width/2.;
                this->y = ofGetHeight() - this->height - this->padding;
            }
            
            auto fontx = this->width/2. - sw/2.;
            auto fonty = this->height/2. - sh/2. + font->getLineHeight()/2.0;
            
            ofPushStyle();
            ofSetColor( isMousePressed() ? colorActive.get() : color.get());
            ofDrawRectangle(*this);
            ofSetColor(fontColor.get());
            font->drawString(myText, this->x + fontx, this->y + fonty);
            ofPopStyle();
        }
    }
    
    void onRelease(int x, int y, int button) {
        ofNotifyEvent(onReleaseEvent, this);
    }
    
    ofParameter<ofColor> color, colorActive, fontColor;
    ofParameter<string> myText;
    ofParameter<float> padding;
    ofParameter<bool> autoCenter;
    ofTrueTypeFont * font;
    
    // events
    ofEvent <void> onReleaseEvent;
    
protected:
};
