//
//  GridMesh.h
//  MMI_Kaleidoscope
//
//  Created by Brett Renfer on 10/8/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class MovingMesh : public ofMesh {
public:
    
    void generate(float width, float height){
        clear();
        append(ofMesh::plane(width, height));
        setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        c.set(1.);
        c.setSaturation(.7);
        c.setBrightness(.7);
        c.setHue(ofRandom( c.limit() ));
    }
    
    void draw(){
        ofPushMatrix();
        ofTranslate(pos);
        ofMesh::draw();
        ofPopMatrix();
    }
    
    ofVec2f pos;
    int whichTex;
    ofFloatColor c;
};

class GridMesh {
public:
    
    GridMesh(){
        // setup gui params
        parameters.setName("Grid");
        stepX.set("Step X", 2, 0, 50);
        stepY.set("Step Y", 2, 0, 50);
        squareSize.set("Square Size", .1, 0., .5);
        spacing.set("Spacing", .1, 0., .1);
        speed.set("Speed", -1., -10, 10.);
        noiseRate.set("Noise Rate", 10, 1,100);
        colorAlpha.set("Noise Alpha", 1., 0.,1.);
        x.set("x", 0., -1., 1.);
        y.set("y", 0., -1., 1.);
        
        parameters.add(stepX);
        parameters.add(stepY);
        parameters.add(squareSize);
        parameters.add(spacing);
        parameters.add(speed);
        parameters.add(noiseRate);
        parameters.add(colorAlpha);
        parameters.add(x);
        parameters.add(y);
        
        pos.set(0,0);
    }
    
    void update(){
        static float sX = 0;
        static float sY = 0;
        static float end = 0;
        
        float sz = squareSize * ofGetWidth();
        float sp = spacing * ofGetWidth();
        float dX = x;
        float dY = y;
        
        if ( sX != stepX || sY != stepY ){
            meshes.clear();
            
            for (int i=0; i<stepX; i++){
                dY = y;
                for (int j=0; j<stepY; j++){
                    meshes.push_back(MovingMesh());
                    meshes.back().generate(sz,sz);
                    meshes.back().pos.set(dX, dY);
                    meshes.back().whichTex = floor(ofRandom(0, textures.size()));
                    dY += sz + sp;
                }
                dX += sz + sp;
            }
            
            end = dX;
        }
        
        sX = stepX;
        sY = stepY;
        
        int ind = 0;
        
        for ( auto & m : meshes ){
            m.pos.x += speed;
            if ( m.pos.x <= -sz ){
                m.pos.x += end + sp;
            }
            ind++;
        }
    }
    
    void draw(){
        ofDisableDepthTest();
//        
        
        ofPushMatrix();
//        int index = 0;
        int idx = 0;
        for ( auto & m : meshes ){
            int index = m.whichTex;
            
            bool bBound = false;
            bool bTexGood = textures.size() > index && textures[index]->getTexture().isAllocated();
            float noiseVal = ofNoise((m.pos.x + (getTime()) ) * (noiseRate/10000) );
            
            ofVec2f t1(0,0);
            ofVec2f t2(1,0);
            ofVec2f t3(0,1);
            ofVec2f t4(1,1);
            
            if ( bTexGood ){
                auto & t = textures[index]->getTexture();
                t.bind();
                bBound = true;
                
                m.clearTexCoords();
                m.clearColors();
                m.addTexCoord( t1 );
                m.addTexCoord( t2 );
                m.addTexCoord( t3 );
                m.addTexCoord( t4 );
                
                for ( auto & t : m.getTexCoords()){
                    auto & tex = textures[index]->getTexture();
                    float mp = MAX(tex.getWidth(), tex.getHeight());
                    t.x *= mp;// * textureScales[index];
                    t.y *= mp;// * textureScales[index];
                }
            }
            
            m.draw();
            
            if ( bTexGood && noiseVal > 0 ){
                m.clearTexCoords();
                m.clearColors();
                
                float a = ofMap(noiseVal, 0., 1., 0., colorAlpha);
                ofFloatColor color;
                color = m.c;
                color.a = a;
                
                for (int i=0; i<4; i++){
                    m.addColor( color );
                    m.draw();
                }
            }
            
            if ( noiseVal > .9 ){
                m.whichTex = floor(ofRandom(0, textures.size()));
            }
            
            if ( bBound ){
                textures[index]->getTexture().unbind();
            }
//            index++;
//            index %= textures.size();
            
            idx++;
        }
        ofPopMatrix();
    }
    
    ofParameterGroup parameters;
    
    void addTexture( ofBaseHasTexture * source ){
        textures.push_back(source);
        textureScales.push_back(1.0);//floor(ofRandom(1,10)));
    }
    
protected:
    ofVec2f pos;
    ofParameter<int> stepX, stepY;
    ofParameter<float> squareSize, x, y, spacing, speed, noiseRate, colorAlpha;
    
    vector<MovingMesh> meshes;
    vector<ofBaseHasTexture * > textures;
    vector<float> textureScales;
};