//
//  Screen.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "Constants.h"

namespace mmi {
    
    class Screen : public ofRectangle {
    public:
        
        Screen(){
        }
        
        ~Screen(){
        }
        
        virtual void setup( Mode myMode ){
            mode = myMode;
        }
        
        virtual void update(){
            
        }
        
        virtual void draw(){
            
        }
        
        Mode getMode() const {
            return mode;
        }
        
        // listens to AppManager
        void onNewMode(){
//            modeChangedTo( AppManager::get().getMode() );
        }
        
        // VO
        void loadSound( string file ){
            sound.load(file);
        }
        
        virtual void onStart(){
            if ( sound.isLoaded())
                sound.play();
        }
        
        virtual void onEnd(){
            sound.stop();
        }
        
    protected:
        
        Mode mode;
        ofColor color;
        ofSoundPlayer sound;
        
        virtual void modeChangedTo( Mode mode ){
            
        }
        
    };
    
}
