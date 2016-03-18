//
//  BlackFlyCamera.cpp
//  CameraTest
//
//  Created by Brett Renfer on 3/18/16.
//  Copyright (c) 2016 __MyCompanyName__. All rights reserved.
//

#include "BlackFlyCamera.h"

namespace mmi {

    //--------------------------------------------------------------
    BlackFlyCamera::~BlackFlyCamera(){
        if ( getIsSetup() ){
            ofRemoveListener(ofEvents().update, this, &BlackFlyCamera::update);
            isSetup = false;
        }
    }

    //--------------------------------------------------------------
    void BlackFlyCamera::setup( string guid, int width, int height, bool bColor ){
        if ( isSetup ){
            ofLogError()<<"[BlackFlyCamera] Camera is already setup! Returning";
            return;
        }
        if ( bColor ){
            /*
             DC1394_COLOR_FILTER_RGGB
             DC1394_COLOR_FILTER_GBRG,
             DC1394_COLOR_FILTER_GRBG,
             DC1394_COLOR_FILTER_BGGR
             */
            camera.setBayerMode(DC1394_COLOR_FILTER_RGGB);
        }
        camera.setFormat7(true, 0); // this is required for the BlackFlys
        camera.setSize(width, height);
        
        // actually connect
        isSetup = false;
        if ( guid == "" ){
            isSetup = camera.setup();;
        } else {
            isSetup = camera.setup(guid);
        }
        
        if ( isSetup ){
            ofAddListener(ofEvents().update, this, &BlackFlyCamera::update);
        }
    }

    //--------------------------------------------------------------
    void BlackFlyCamera::update( ofEventArgs & e ){
        auto v = camera.grabVideo(capture);
        
        if(v) {
            capture.update();
        } else {
            
        }
    }

    //--------------------------------------------------------------
    void BlackFlyCamera::draw( int x, int y ){
        capture.draw(x, y);
    }
    
    //--------------------------------------------------------------
    void BlackFlyCamera::drawDebug(int x, int y ){
        this->draw(x,y);
        ofDrawBitmapStringHighlight("Camera GUID:"+camera.getGuid(), x, y + 20);
    }

    //--------------------------------------------------------------
    bool BlackFlyCamera::getIsSetup(){
        return isSetup;
    }

    //--------------------------------------------------------------
    ofxLibdc::PointGrey & BlackFlyCamera::getCamera(){
        return camera;
    }

    //--------------------------------------------------------------
    ofImage & BlackFlyCamera::getImage(){
        return capture;
    }
    
}