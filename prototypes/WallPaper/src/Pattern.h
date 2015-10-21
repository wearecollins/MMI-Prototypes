//
//  Pattern.h
//  WallPaper
//
//  Created by Brett Renfer on 5/27/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

namespace MMI {
    
    enum DrawMode {
        MODE_RANDOM = 0,
        MODE_SPIRAL
    };

class Pattern {
public:
    
    void setup( string directory, int tileSize = 256 ){
        // allocate FBO
        resize(tileSize);
        redraw();
        
        // load SVGs
        ofDirectory dir;
        int n = dir.listDir( directory );
        for ( int i=0; i<n; i++){
            svgs.push_back(ofxSVG());
            svgs.back().load( dir.getPath(i));
        }
        
        drawPoints.push_back(ofVec2f(.5 ,0));
        drawPoints.push_back(ofVec2f(.5 ,0));
        drawPoints.push_back(ofVec2f(.5 ,-1.));
        drawPoints.push_back(ofVec2f(.5 ,1.));
        drawPoints.push_back(ofVec2f(1.5 ,0));
        drawPoints.push_back(ofVec2f(-.5 ,0));
        
        mode = MODE_SPIRAL;
        colorMode = 2;
        colorVariance = .3;
    }
    
    void redraw(){
        bUpdate = true;
        colorMode = floor(ofRandom(1,3));
        colorVariance = ofRandom(.1,.5);
    }
    
    void update(){
        if ( bUpdate ){
            bUpdate = false;
            
            tileSize = ofRandom(100,1024);
            resize(tileSize);
            
            tile.begin();
            
            int w = floor(ofRandom(0, svgs.size()));
            ofColor c = svgs[w].getPathAt(0).getFillColor();
            ofClear(c);
            
            switch ( mode ){
                case MODE_RANDOM:
                {
                    int ran = ofRandom(3, 10);
                    for ( int i=0; i<ran; i++){
                        int w = floor(ofRandom(0, svgs.size()));
                        float x = ofRandom( tile.getWidth() * .6 );
                        float y = ofRandom( tile.getWidth() );
                        float r = ofRandom(360.);
                        float s = ofRandom(.2,2.);
                        
                        vector<ofColor> cM;
                        for (int i=0; i<svgs[w].getNumPath(); i++){
                            cM.push_back(c);
                        }
                        if ( colorMode == 0 ){
                            // nuffin'
                            
                        } else if ( colorMode == 1 ){
                            for ( auto & c : cM ){
                                c.setSaturation(c.getSaturation() * ofRandom(1.0 - colorVariance,1.0 + colorVariance));
                            }
                            
                        } else if ( colorMode == 2 ){
                            for ( auto & c : cM ){
                                c.setSaturation(c.getSaturation() * ofRandom(1.0 - colorVariance,1.0 + colorVariance));
                            }
                        }
                        
                        for ( auto & p : drawPoints ){
                            ofPushMatrix();
    //                        ofScale(.5,.5);
                            ofTranslate(p * ofVec2f(tile.getWidth(), tile.getHeight()));
                            ofTranslate(x,y);
                            ofRotateZ(r);
                            ofScale(s,s);
                            if (colorMode == 0 ){
                                svgs[w].draw();
                            } else {
                                int n = svgs[w].getNumPath();
                                for(int i = 0; i < n; i++){
                                    ofPath p = svgs[w].getPathAt(i); // not reference!
                                    p.setFillColor(cM[i]);
                                    p.draw();
                                }
                            }
                                
                            ofPopMatrix();
                        }
                    }
                }
                    break;
                    
                case MODE_SPIRAL:
                {
                    int ranN = ofRandom(2, 5);
                    
                    for ( int j=0; j<ranN; j++){
                        int w = floor(ofRandom(0, svgs.size()));
                        int ran = ofRandom(10, 90);
                        
                        for ( int i=ran; i<360; i++){
                            float x = ofRandom( tile.getWidth() * .5 );
                            float y = ofRandom( tile.getWidth() * .5 );
                            float r = i;
                            float s = ofRandom(.5,1.5);
                            
                            vector<ofColor> cM;
                            for (int i=0; i<svgs[w].getNumPath(); i++){
                                cM.push_back(c);
                            }
                            if ( colorMode == 0 ){
                                // nuffin'
                                
                            } else if ( colorMode == 1 ){
                                for ( auto & c : cM ){
                                    c.setSaturation(c.getSaturation() * ofRandom(1.0 - colorVariance,1.0 + colorVariance));
                                }
                                
                            } else if ( colorMode == 2 ){
                                for ( auto & c : cM ){
                                    c.setSaturation(c.getSaturation() * ofRandom(1.0 - colorVariance,1.0 + colorVariance));
                                }
                            }
                            
    //                        for ( auto & p : drawPoints ){
                                ofPushMatrix();
                                ofTranslate(tile.getWidth()/2.0, tile.getHeight()/2.0);
                                ofRotateZ(r);
                                //                        ofScale(.5,.5);
    //                            ofTranslate(p * ofVec2f(tile.getWidth(), tile.getHeight()));
                                ofTranslate(x,y);
                                ofScale(s,s);
                                if (colorMode == 0 ){
                                    svgs[w].draw();
                                } else {
                                    int n = svgs[w].getNumPath();
                                    for(int i = 0; i < n; i++){
                                        ofPath p = svgs[w].getPathAt(i); // not reference!
                                        p.setFillColor(cM[i]);
                                        p.draw();
                                    }
                                }
                                
                                ofPopMatrix();
    //                        }
                        }
                    }
                }
                    break;
            }
            
            
            tile.end();
        }
    }
    
    void draw(){
        float gridW = (float) ceil(ofGetWidth() / tile.getWidth());
        float gridH = (float) ceil(ofGetWidth() / tile.getWidth());
        
        //
        for ( int x=0; x<gridW; x++){
            for ( int y=0; y<gridH; y++){
                tile.draw(x * tile.getWidth(), y * tile.getWidth());
            }
        }
    }
    
    void resize( int tileSize ){
        tile.allocate(tileSize, tileSize);
    }
    
    int colorMode;
    float colorVariance;
    
protected:
    
    ofFbo tile;
    vector<ofxSVG> svgs;
    vector<ofVec2f> drawPoints;
    bool bUpdate;
    DrawMode mode;
    int tileSize;
};

}