//
//  IntroScreen.h
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

    class IntroScreen : public Screen {
    public:

        void setup( Mode myMode ){
            Screen::setup(myMode);
            
            button.font = &ofxAssets::font(FONT_HEAD, FONT_BUTTON_SIZE);
            button.color.set(BUTTON_COLOR);
            button.colorActive.set(BUTTON_COLOR_ACTIVE);
            button.myText.set("Get Started");
        }
        
        void draw(){
            ofxAssets::font(FONT_HEAD, FONT_HEAD_SIZE).drawString("Perform a Puppet\non Television", 20, 20 + FONT_HEAD_SIZE);
            
            button.draw();
        }
        
        TextButton button;
    };

}