//
//  Clip.h
//  EW_Perf_Camera
//
//  Created by Brett Renfer on 2/2/16.
//
//

#pragma once

#include "ofMain.h"

static string clip_bg_location = "backgrounds";
static string clip_location = "clips";

class Clip {
public:
    
    string name;
    string backgroundImage;
    
    ofImage background;
    
    void setup( string name ){
        this->name = name;
        background.load(clip_bg_location + "/" + name + ".png");
    }
    
    string getClip(){
        return clip_location + "/" + name;
    }
};

static map<string, Clip> clips;
