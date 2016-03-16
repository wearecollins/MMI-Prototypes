//
//  TriangleGridMesh.h
//  MMI_Kaleidoscope
//
//  Created by Brett Renfer on 10/7/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class TriangleGrid : public ofMesh {
public:
    
    void generate(float x, float y, int stepX, int stepY, float triangleSize ){
        clear();
        
        bool bAltY = false;
        float tX = x;
        float tY = y;
        
        for ( int j=0; j<stepY-1; j++){
            bool bAltX = bAltY;
            tX = x;
            
            for ( int i=0; i<stepX-1; i++){
                if (bAltX){
                    addVertex(ofVec2f(tX, tY));
                    addVertex(ofVec2f(tX + triangleSize, tY));
                    addVertex(ofVec2f(tX + triangleSize, tY + triangleSize));
                    
                    addVertex(ofVec2f(tX, tY));
                    addVertex(ofVec2f(tX + triangleSize, tY + triangleSize));
                    addVertex(ofVec2f(tX, tY + triangleSize));
                    
                } else {
                    addVertex(ofVec2f(tX, tY));
                    addVertex(ofVec2f(tX + triangleSize, tY));
                    addVertex(ofVec2f(tX, tY + triangleSize));
                    
                    addVertex(ofVec2f(tX + triangleSize, tY));
                    addVertex(ofVec2f(tX + triangleSize, tY + triangleSize));
                    addVertex(ofVec2f(tX, tY + triangleSize));
                }
                tX += triangleSize;
            }
            bAltY = !bAltY;
            tY += triangleSize;
        }
        
        // setup normalized tex coords
        for ( auto & v : getVertices()){
            auto tc = ofVec2f(v);
            tc.x /= (stepX * triangleSize);
            tc.y /= (stepY * triangleSize);
            addTexCoord(tc);
        }
        
        setupIndicesAuto();
    }
};

class TriangleGridMesh {
public:
    
    TriangleGridMesh(){
        // setup gui params
        parameters.setName("TriangleGrid");
        stepX.set("Step X", 2, 0, 50);
        stepY.set("Step Y", 2, 0, 50);
        triangleSize.set("Triangle Size", .1, 0., .5);
        x.set("x", 0., -1., 1.);
        y.set("y", 0., -1., 1.);
        
        parameters.add(x);
        parameters.add(y);
        parameters.add(stepX);
        parameters.add(stepY);
        parameters.add(triangleSize);
    }
    
    void update(){
        triangles.clear();
        
        for (int i=0; i<textures.size(); i++){
            triangles.push_back(TriangleGrid());
            triangles.back().generate(x * ofGetWidth(),y * ofGetHeight(),
                                      stepX, stepY, triangleSize * (ofGetWidth()) );
        
            int z = 0;
            ofFloatColor c;
            
            for ( auto & ind : triangles.back().getIndices()){
                if ( z == 0 ){
                    auto v = triangles.back().getVertex(ind);
                    float n = ofSignedNoise(v + (getTime()) * .00005 + i * 3.);
                    c = ofFloatColor(1., n > 0 ? round(n) : 0 );
                }
                triangles.back().addColor( c );
                z++;
                if ( z == 3 ){
                    z= 0;
                }
            }
        
        }
    }
    
    void draw(){
        ofPushMatrix();
        int index = 0;
        for ( auto & m : triangles ){
            bool bBound = false;
            if ( textures.size() > index && textures[index]->getTexture().isAllocated() ){
                auto & t = textures[index]->getTexture();
                t.bind();
                bBound = true;
                
                for ( auto & t : m.getTexCoords()){
                    auto & tex = textures[index]->getTexture();
                    t *= ofVec2f( tex.getWidth() * textureScales[index], tex.getWidth() * textureScales[index] );
                    t.x = ofWrap(t.x, 0, tex.getWidth()-1);
                    t.y = ofWrap(t.y, 0, tex.getHeight()-1);
                }
                
            }
            m.draw();
            if ( bBound ){
                textures[index]->getTexture().unbind();
            }
            index++;
            index %= textures.size();
        }
        ofPopMatrix();
    }
    
    
    void addTexture( ofBaseHasTexture * source ){
        textures.push_back(source);
        textureScales.push_back(floor(ofRandom(1,10)));
    }
    
    ofParameterGroup parameters;
    
protected:
    ofParameter<int> stepX, stepY;
    ofParameter<float> triangleSize, x, y;
    
    vector<TriangleGrid> triangles;
    vector<ofBaseHasTexture * > textures;
    vector<float> textureScales;
    
};