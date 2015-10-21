//
//  Grid.h
//  IntroWallGrid
//
//  Created by Brett Renfer on 10/15/15.
//
//

#pragma once

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

class TexCoords {
public:
    
    void setup( vector<ofVec3f> incoords ){
        origin.set(FLT_MAX, FLT_MAX);
        dims.set(-FLT_MAX, -FLT_MAX);
        
        for ( auto & v : incoords ){
            if ( v.x < origin.x ){
                origin.x = v.x;
            }
            if ( v.y < origin.y ){
                origin.y = v.y;
            }
            if ( v.x > dims.x ){
                dims.x = v.x;
            }
            if ( v.y > dims.y ){
                dims.y = v.y;
            }
        }
        
        dims -= origin;
        
        for ( auto & v : incoords ){
            auto tc = v - origin;
            tc /= dims;
            coords.push_back(tc);
        }
    }
    
    vector<ofVec2f> getScaled( ofTexture & tex ){
        vector<ofVec2f> ret;
        float min = MIN( tex.getWidth(), tex.getHeight());
        float max = MIN( tex.getWidth(), tex.getHeight());
        float scale = max / min;
        for ( auto c : coords ){
            c.x *= (tex.getWidth() == max ? max : max * scale);
            c.y *= (tex.getWidth() == max ? max * scale : max );
            ret.push_back(c);
        }
        return ret;
    }
    
    vector<ofVec2f> coords;
    ofVec2f origin, dims;
};

class Grid {
public:
    
    Grid (){
        // setup gui params
        parameters.setName("Grid");
        speed.set("Speed", -1., -10, 10.);
        noiseRate.set("Noise Rate", 10, 1,100);
        colorAlpha.set("Noise Alpha", 1., 0.,1.);
        
        parameters.add(speed);
        parameters.add(noiseRate);
        parameters.add(colorAlpha);
        
        position.set(0,0);
    }
    
    virtual void addTexture( ofBaseHasTexture * source ){
        textures.push_back(source);
        textureScales.push_back(1.0);//floor(ofRandom(1,10)));
    }
    
    virtual void update(){
        position.x += speed;
        
        static uint64_t last = 0;
        static float rate = 500;
        auto t = ofGetElapsedTimeMillis();
        
        if ( t - last > rate ){
            featured = floor( ofRandom(1, meshes.size() / 10. ));
            last = t;
            rate = ofRandom(500, 3000);
        }
    }
    
    virtual void draw(){
        ofPushMatrix();
        ofTranslate(position);
        int index = 0;
        int i = 0;
        
        for ( auto & m : meshes ){
            // are you on screen
            bool bVisible = false;
            float pos = -FLT_MAX;
            for ( auto & v : m.getVertices() ){
                if ( pos  == -FLT_MAX ){
                    pos = (v.x * v.y) + colors[i].r * 10.;
                }
                if ( v.x - position.x > 0 && v.x + position.x < ofGetWidth() ){
                    bVisible = true;
                    break;
                }
            }
            
            if (bVisible){
                
                float noiseVal = ofSignedNoise((pos + (getTime()) ) * (noiseRate/10000) );
                
                bool bBound = false;
                if ( textures.size() > index && textures[index]->getTexture().isAllocated() ){
                    textures[index]->getTexture().bind();
                    bBound = true;
                    
                    auto tc = texCoords[i].getScaled( textures[index]->getTexture() );
                    m.clearColors();
                    m.clearTexCoords();
                    m.addTexCoords(tc);
                    
                    ofFloatColor c(1.);
                    c.a = .5;//noiseVal < 0 ? ofMap(-noiseVal, 0., 1., 0., .25) : .25;
                    
                    bool bFeatured = i == 0 ? false : (featured % i == 0 || texCoords[i].dims.x > 300);
                    
                    if ( !bFeatured ){
                        for ( auto & v : m.getVertices() ){
                            m.addColor( c );
                        }
                    }
                    
                    m.draw();
                    colors[i].a = MAX(0, noiseVal );
                    
                    if (bBound){
                        textures[index]->getTexture().unbind();
                    }
                    
                    if ( !bFeatured  ){
                        m.clearColors();
                        m.clearTexCoords();
                        for ( auto & v : m.getVertices() ){
                            m.addColor( colors[i] );
                        }
                        m.draw();
                    }
                }
            }
            
            // all off screen
            
            int n = 0;
            int off = 0;
            for ( auto & v : m.getVertices() ){
                if ( v.x + position.x < 0 ){
                    off++;
                }
                n++;
            }
            
            // hack: all off to the right...
            if ( n == off ){
                for ( auto & v : m.getVertices() ){
                    v.x += totalWidth;
                }
            }
            
            index ++;
            index %= textures.size();
            i++;
        }
        ofPopMatrix();
    }
    
    ofParameterGroup parameters;
    
protected:
    vector<ofFloatColor> colors;
    vector<ofMesh> meshes;
    vector<TexCoords> texCoords;
    vector<ofBaseHasTexture * > textures;
    vector<float> textureScales;
    ofParameter<float> speed, noiseRate, colorAlpha;
    ofVec2f position;
    float totalWidth;
    
    int featured;
};