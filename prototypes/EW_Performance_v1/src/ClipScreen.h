//
//  ClipScreen.h
//  EW_Performance
//
//  Created by Brett Renfer on 7/8/15.
//
//

#pragma once

#include "ofMain.h"
#include "Screen.h"
#include "AppManager.h"
#include "ofxAssets.h"

namespace mmi {
    
    class Clip : public ofRectangle {
        public:
            
        bool setup( string path, string name, string perf );
        void draw();
        void draw( float x, float y, float w=0, float h=0 );
        
        ofVideoPlayer & v();
        
        bool bOver;
            
    protected:
        
        ofVideoPlayer movie;
        string title, performers;
    };

    class ClipScreen : public Screen {
    public:
        
        void setup( Mode myMode );
        
        void update();
        
        void draw();
        void drawPreview();
        
        // for mouse
        void setPos( float x, float y, float s);
        void onMousePressed( ofMouseEventArgs & e );
        void onMouseMoved( ofMouseEventArgs & e );
        
        // build / teardown
        void onStart();
        
        void onEnd();
        
        Clip & getCurrentClip(){
            return *currentClip;
        }
        
    protected:
        
        vector<Clip> clips;
        Clip * currentClip;
        ofVec3f mPos;
    };
}