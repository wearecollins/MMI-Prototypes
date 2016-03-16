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
    
protected:
    
    ofAVFoundationPlayer * player;
    ofFbo * kinect;
    
    bool bUpdateMe;
    void threadedFunction();
    
    vector<ofAVFoundationPlayer *> videos;
    int gridX, gridY, drawCount, dGridX, dGridY;
    ofFbo render, videoFbo;
    ofShader renderShader;
    
    bool bUseSpriteSheets;
};
