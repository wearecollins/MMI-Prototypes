//
//  BlackFlyCamera.h
//  CameraTest
//
//  Created by Brett Renfer on 3/18/16.
//

#pragma once

#include "ofMain.h"
#include "ofxLibdc.h"

namespace mmi {
    
    class BlackFlyCamera
    {
        friend class ofxLibdc::Camera;
    public:
        
        ~BlackFlyCamera();
        void setup( int width = 960, int height = 600, string guid = "" );
        void update( ofEventArgs & e );
        void draw(int x, int y );
        void drawDebug(int x, int y );
        
        bool getIsSetup();
        ofxLibdc::PointGrey & getCamera();
        ofImage & getImage();
        
    protected:
        bool isSetup;
        ofxLibdc::PointGrey camera;
        ofImage capture;
    };

}
