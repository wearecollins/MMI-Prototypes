//
//  CameraManager.h
//  EW_Performance
//
//  Created by Brett Renfer on 1/28/16.
//
//

#pragma once

#include "ofMain.h"

namespace mmi {
    class Camera : public ofBaseHasTexture {
    public:
        
        Camera(){
            bIsOpen = 0;
            camWidth = 640;
            camHeight = 480;
            camFrameRate = 30;
        }
        
        virtual void setup( int idx = 0, string name = "Camera 1" ){
            params.setName(name);
        }
        
        // returns isFrameNew
        virtual void update(){
            bIsFrameNew = false;
        }
        
        virtual void draw( float x, float y, float w=-1, float h=-1 ){
            tex.draw(x,y,w == -1 ? camWidth : w,h == -1 ? camHeight : h);
        }
        
        // call before setup
        void setDimensions( int w, int h ){
            camWidth = w;
            camHeight = h;
        }
        
        void setFrameRate( int fr ){
            camFrameRate = fr;
        }
        
        ofParameterGroup params;
        
        // camera stuff
        bool isFrameNew() const {
            return bIsFrameNew;
        }
        
        // need these for ofBaseHasTexture
        void setUseTexture(bool bUseTex){
            //idgaf
        }
        
        bool isUsingTexture() const{
            return true;
        }
        
        virtual const ofTexture & getTexture() const{
            return tex;
        }
        
        virtual ofTexture & getTexture(){
            return tex;
        }
        
    protected:
        
        int camWidth;
        int camHeight;
        int camFrameRate;
        
        bool bIsOpen;
        bool bIsFrameNew;
        
        ofTexture tex;
    };

    class CameraManager {
    public:
        
        
    };
}
