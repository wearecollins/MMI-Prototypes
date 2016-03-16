//
//  PreviewScreen.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "Screen.h"
#include "ofxAssets.h"
#include "TextButton.h"

namespace mmi {

class PreviewScreen : public Screen {
public:
    
    void setup( Mode myMode ){
        Screen::setup(myMode);
        
        button.font = &ofxAssets::font(FONT_HEAD, FONT_BUTTON_SIZE);
        button.color.set(BUTTON_COLOR);
        button.colorActive.set(BUTTON_COLOR_ACTIVE);
        button.myText.set("Next");
    }
    
    void setTexture( ofTexture & tex ){
        this->tex = &tex;
    }
    
    void loadSound( string file ){
        sound.load(file);
    }
    
    
    void draw(){
        // assuming tex = camera ;0
        
        float scale = ofGetWidth()/tex->getWidth();
        ofPushMatrix();
//        ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
        ofScale(scale,scale);
//        ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
        tex->draw(0,0);
        ofPopMatrix();
        
        ofxAssets::font(FONT_HEAD, FONT_HEAD_SIZE).drawString("Overview", 20, 20 + FONT_HEAD_SIZE);
        
        button.draw();
    }
    
    
    TextButton button;
    
protected:
    
    ofTexture * tex;
};
    
}
