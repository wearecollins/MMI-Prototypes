#pragma once

#include "ofxSvg.h"

class SvgSampler {
public:
    
    void load( string svgFile ){
        svg.load(svgFile);
        
        int n = svg.getNumPath();
        for ( int i=0; i<n; i++){
            colors.push_back( svg.getPathAt(i).getFillColor() );
        }
    }
    
    
    const vector<ofColor> & getColors(){
        return colors;
    }
    
    int getNumColors(){
        return colors.size();
    }
    
    ofColor getRandomColor(){
        if ( getNumColors() > 0 ){
            return colors[ floor(ofRandom(0, getNumColors()))];
        } else {
            static bool warned = false;
            if (!warned) ofLogWarning()<<"No colors loaded";
            return ofColor(255);
        }
    }
    
protected:
    
    ofxSVG svg;
    vector<ofColor> colors;
    
    bool bLoaded;
    
};