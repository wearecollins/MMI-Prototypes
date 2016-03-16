//
//  KaleidoscopeMesh.h
//  MMI_Kaleidoscope
//
//  Created by Brett Renfer on 10/6/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

#define SQRT_THREE sqrt(3)

static float getETHeight( float side ){
    return (SQRT_THREE / 2.0) * side;
}

class KMesh : public ofMesh {
public:
    int textureIndex;
    
    void generate( float x, float y, float triangleSize, int circleRes, float a = 0., bool bDoKaleido = true){
        bool bNormalTexCoords = true;
        float normalVal = 1.;
        
        clear();
        
        int inc = 360/circleRes;
        
        bool bAlt = false;
        
        ofVec2f t1,t2,t3; // L T R
        ofVec2f c;
        
        if ( bNormalTexCoords ){
            t1.set(0.,getETHeight(triangleSize)/triangleSize * normalVal);
            t2.set(.5 * normalVal,0.);
            t3.set(normalVal, getETHeight(triangleSize)/triangleSize * normalVal);
            
        } else {
            t1.set(0.,getETHeight(triangleSize));
            t2.set(triangleSize/2.,0.);
            t3.set(triangleSize,getETHeight(triangleSize));
        }
        
        c.x = (t1.x + t2.x + t3.x)/3.;
        c.y = (t1.y + t2.y + t3.y)/3.;
        
        t1.rotate(a, c);
        t2.rotate(a, c);
        t3.rotate(a, c);
        
        for ( int i=0; i<360; i+= inc ){
            auto a = ofDegToRad(i);
            auto a2 = ofDegToRad(i + inc);
            auto v1 = ofVec2f( -cos(a), -sin(a) );
            auto v2 = ofVec2f( -cos(a2), -sin(a2) );
            auto v3 = ofVec2f(0,0);
            addVertex( v1 );
            addVertex( v2 );
            addVertex( v3 );
            
            if ( bDoKaleido ){
                addTexCoord( bAlt ? t3 : t1 );
                addTexCoord( bAlt ? t1 : t3 );
                addTexCoord( t2 );
                bAlt = !bAlt;
            } else {
                addTexCoord( v1 );
                addTexCoord( v2 );
                addTexCoord( v3 );
            }
        }
        setupIndicesAuto();
        
        for ( auto & v : getVertices()){
            v *= triangleSize;
            v += ofVec2f(x,y);
        }
        if ( !bDoKaleido ){
            for ( auto & v : getTexCoords()){
                v.x = ofMap(v.x,-1,1, 0,1);
                v.y = ofMap(v.y,-1,1, 0,1);
            }
        } else if ( bNormalTexCoords){
//            for ( auto & v : getTexCoords()){
//                v.x = ofClamp(v.x, 0.,normalVal);
//                v.y = ofClamp(v.x, 0.,normalVal);
//            }
        }
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

class KaleidoscopeMesh {
    
public:
    
    KaleidoscopeMesh(){
        // setup gui params
        parameters.setName("Kaleidoscope");
        stepX.set("Step X", 2, 0, 50);
        stepY.set("Step Y", 2, 0, 50);
        triangleSize.set("Triangle Size", .1, 0., .5);
        circleRes.set("Circle Resolution", 6, 2, 10);
        numTextures.set("Number textures", 1, 0, 10);
        noiseRate.set("Noise Rate", 10, 1,100);
        speed.set("Speed", -1., -10, 10.);
        doKaleidoscope.set("Kaleidoscope", true);
        x.set("x", 0., -1., 1.);
        y.set("y", 0., -1., 1.);
        
        parameters.add(x);
        parameters.add(y);
        parameters.add(stepX);
        parameters.add(stepY);
        parameters.add(circleRes);
        parameters.add(triangleSize);
        parameters.add(noiseRate);
        parameters.add(speed);
        parameters.add(doKaleidoscope);
        
        stepX.addListener(this, &KaleidoscopeMesh::onChangedI);
        stepY.addListener(this, &KaleidoscopeMesh::onChangedI);
        numTextures.addListener(this, &KaleidoscopeMesh::onChangedI);
        
        triangleSize.addListener(this, &KaleidoscopeMesh::onChangedF);
        x.addListener(this, &KaleidoscopeMesh::onChangedF);
        y.addListener(this, &KaleidoscopeMesh::onChangedF);
        
        bUpdate = true;
        bRotating = true;
    }
    
    ~KaleidoscopeMesh(){
        stepX.removeListener(this, &KaleidoscopeMesh::onChangedI);
        stepY.removeListener(this, &KaleidoscopeMesh::onChangedI);
        numTextures.removeListener(this, &KaleidoscopeMesh::onChangedI);
        
        triangleSize.removeListener(this, &KaleidoscopeMesh::onChangedF);
        x.removeListener(this, &KaleidoscopeMesh::onChangedF);
        y.removeListener(this, &KaleidoscopeMesh::onChangedF);
    }
    
    void update(){
        static float sX = 0;
        static float sY = 0;
        static float end = 0;
        float dX = x;
        float dY = y;
        float tS = triangleSize * ofGetWidth();
        float nX = x * ofGetWidth();
        float nY = y * ofGetHeight();
        
        if ( sX != stepX || sY != stepY ){
            mesh.clear();
            
            bool bAlt = false;
            
            float t = getTime() * ( noiseRate / 10000);
            
            static bool bP = false;
            
            for ( float j=0; j<stepY; j++){
                for ( float i=0; i<stepX; i++){
                    mesh.push_back(KMesh());
                    float kx = nX + i * (tS * 1.5);
                    float ky = nY + j * (getETHeight(tS) * 2);
                    if ( bAlt ){
                        ky += getETHeight(tS);
//                        kx -= tS/2.;
                    }
                    
                    float a = ofNoise((kx + ky) + t);
                    
                    bool bK = doKaleidoscope ? ( ((int) j) % 2 == 0 ? true : false) : false;
                    
                    mesh.back().generate(kx, ky, tS, circleRes, getTime() * .01, bK);
                    mesh.back().clearColors();
                    
                    for ( auto & ind : mesh.back().getVertices()){
                        mesh.back().addColor( ofFloatColor(1., ofMap(a, 0., 1., .5, 1.) ) );
                    }
                    bAlt = !bAlt;
                }
            }
            
            bP = true;
        }
        bUpdate = false;
        int ind = 0;
        for ( auto & m : mesh ){
            m.pos.x += speed;
            if ( m.pos.x <= -tS ){
                m.pos.x += nX + stepX * (tS * 1.5);
            }
            ind++;
        }
    }
    
    
    void draw(){
        draw( ofGetWidth(), ofGetHeight());
    }
    
    void draw( float width, float height ){
        ofPushMatrix();
//        ofTranslate(x.get() * width, y.get() * height);
        int index = 0;
        for ( auto & m : mesh ){
            bool bBound = false;
            if ( textures.size() > index && textures[index]->getTexture().isAllocated() ){
                textures[index]->getTexture().bind();
                bBound = true;
                
                for ( auto & t : m.getTexCoords()){
                    auto & tex = textures[index]->getTexture();
                    t *= ofVec2f( tex.getHeight(), tex.getHeight());
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
    
    void flagNeedToUpdate(){
        bUpdate = true;
    }
    
    void addTexture( ofBaseHasTexture * source ){
        textures.push_back(source);
    }
    
    ofParameterGroup parameters;
    ofParameter<int> stepX, stepY, circleRes, numTextures;
    ofParameter<float> triangleSize, x, y, noiseRate, speed;
    ofParameter<bool> doKaleidoscope;
    
protected:
    
    ofVec2f pos;
    vector<KMesh> mesh;
    vector<ofBaseHasTexture * > textures;
    
    bool bUpdate, bRotating;
    
    void onChangedI( int & v ){ flagNeedToUpdate(); };
    void onChangedF( float & v ){ flagNeedToUpdate(); };
};