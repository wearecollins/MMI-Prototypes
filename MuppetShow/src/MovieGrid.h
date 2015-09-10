//
//  MovieGrid.h
//  MuppetShow
//
//  Created by Brett Renfer on 5/21/15.
//
//

#pragma once

#include "ofMain.h"

class MovieGrid : protected ofThread {
public:
    
    ~MovieGrid(){
        waitForThread();
    }
    
    void setup( ofFbo & kinect, string directory, string fmt = "mp4");
    void update();
    void draw();
    
    void reload();
    void resize();
    
    void setActive ( int index );
    void setInactive( int index );
    
    void rewind();
    
protected:
    
    ofPtr<ofAVFoundationPlayer> player;
    ofFbo * kinect;
    
    ofFbo activationTexture;
    float * activeData;
    vector<bool>active;
    
    bool bUpdateMe;
    void threadedFunction();
    
    vector<ofPtr<ofAVFoundationPlayer> > videos;
    int gridX, gridY, drawCount, dGridX, dGridY;
    ofFbo render, renderFinal,videoFbo;
    ofShader renderShader;
    
    bool bUseSpriteSheets;
};
