//
//  Kinect2Utils.h
//  example_kinectv2
//
//  THESE ARE FROM THE OFXMULTIKINECTV2 LIB
//  I JUST GET TIRED OF ADDING TO ofApp.h, OK?

#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"

#define KINECT2_DEPTH_X             512
#define KINECT2_DEPTH_Y             424
#define KINECT2_Z_AXIS_MAGNITUDE    .5

#define STRINGIFY(x) #x

static string depthFragmentShader =
STRINGIFY(
          uniform vec3 colorLow;
          uniform vec3 colorHigh;
          uniform float low1;
          uniform float high1;
          uniform sampler2DRect tex;
          
          float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
              float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
          
              if(outputMax < outputMin){
                  if( outVal < outputMax )outVal = outputMax;
                  else if( outVal > outputMin )outVal = outputMin;
              }else{
                  if( outVal > outputMax )outVal = outputMax;
                  else if( outVal < outputMin )outVal = outputMin;
              }
              return outVal;
          }
          
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r;
              float low2 = 1.0;
              float high2 = 0.0;
              float d = ofMap(value, low1, high1, low2, high2);
              //clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
              
              if (col.r < low1 || col.r > high1) {
                  d = 0.0;
                  gl_FragColor = vec4(vec3(0.), 1.0);
              } else {
                  vec3 outColor = colorLow;
                  outColor.r = ofMap(d, 0., 1., colorLow.r, colorHigh.r);
                  outColor.g = ofMap(d, 0., 1., colorLow.g, colorHigh.g);
                  outColor.b = ofMap(d, 0., 1., colorLow.b, colorHigh.b);
                  
                  gl_FragColor = vec4(outColor, 1.0);
              }
          }
          );

static string irFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r / 65535.0;
              gl_FragColor = vec4(vec3(value), 1.0);
          }
          );

class Kinect2Mesh : protected ofThread {
public:
    
    void setup( bool threaded = false ){
        bThreaded = threaded;
        
        verts.resize( KINECT2_DEPTH_X * KINECT2_DEPTH_Y );
//        mesh.addVertices(verts);
        mesh.setMode( OF_PRIMITIVE_POINTS );
        near = 0.;
        far = 50000.;
    }
    
    void update( ofFloatPixels & depthPixelsRef ){
        
        float time = ofGetElapsedTimeMillis() * .0001;
        
        static float map_scale_factor = 2.; // where do i put you?
        mesh.clear();
        for (int y = 0; y < KINECT2_DEPTH_Y; y++) {
            for (int x = 0; x < KINECT2_DEPTH_X; x++) {
                int index = x + (y*KINECT2_DEPTH_X);
                float z = KINECT2_Z_AXIS_MAGNITUDE * depthPixelsRef.getData()[index];
                
                // IGNORE THE HIGH AND LOW VALUES
                if ((z == 0) || (z == KINECT2_Z_AXIS_MAGNITUDE) || (z < near) || ( z > far ) ) continue;
                
                // CREATE A POINT WITH THE SAME X AND Y BY A Z VALUE MATCHING THE DEPTH
                ofVec3f newPoint(x - KINECT2_DEPTH_X / 2, (KINECT2_DEPTH_Y / 2) - y, z);
                ofFloatColor c;
                c.setSaturation(1.);
                c.setBrightness(1.);
                c.setHue( ofWrap( time + ofMap(z, near, far, 0, 1.), 0., 1.));
                mesh.addColor(c);
                mesh.addVertex(newPoint);
                mesh.addTexCoord( ofVec2f( ofMap(x,0,KINECT2_DEPTH_X,0., KINECT2_DEPTH_X * map_scale_factor),
                                          ofMap(y,0,KINECT2_DEPTH_Y,0., KINECT2_DEPTH_Y * map_scale_factor)
                                          ));
            }
        }
    }
    
    ofMesh & getMesh(){
        return mesh;
    }
    
    void draw(){
        mesh.drawVertices();
    }
    
    // public externals
    ofParameter<float> near, far;
    
protected:
    
    bool bThreaded;
    
    ofMesh mesh;
    vector<ofVec3f> verts;
    vector<ofVec2f> texCoords;
    
    void threadedFunction(){
        while( isThreadRunning()){
            
        }
    }
    
};

class Kinect2Manager {
public:
    
    void setup(){
        // setup params
        kinectLow.set("Kinect Depth Low", 500., 0., 5000.);
        kinectHigh.set("Kinect Depth High", 500., 0., 5000.);
        depthColorLow.set("Kinect Color Low", ofFloatColor(0), ofFloatColor(0), ofFloatColor(1.));
        depthColorHigh.set("Kinect Color High", ofFloatColor(1.), ofFloatColor(0), ofFloatColor(1.));
        bDrawColor.set("Kinect Draw Color", true);
        bDrawDepth.set("Kinect Draw Depth", true);
        bDrawIr.set("Kinect Draw IR", true);
        
        // load shaders
        depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
        depthShader.linkProgram();
        
        irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
        irShader.linkProgram();
        
        kinect0.open(true, true, 0);
        
        // Note :
        // Default OpenCL device might not be optimal.
        // e.g. Intel HD Graphics will be chosen instead of GeForce.
        // To avoid it, specify OpenCL device index manually like following.
        // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
        
        kinect0.start();
    }
    
    // return isFrameNew val
    bool update(){
        kinect0.update();
        if (kinect0.isFrameNew()) {
            colorTex0.loadData(kinect0.getColorPixelsRef());
            depthTex0.loadData(kinect0.getDepthPixelsRef());
            irTex0.loadData(kinect0.getIrPixelsRef());
            
            if ( colorTex0.getWidth() != 0 ){
                return true;
            }
        }
        return false;
    }
    
    // debug draw
    void draw()
    {
        if (colorTex0.isAllocated() && bDrawColor) {
            colorTex0.draw(0, 0);
        }
        if (depthTex0.isAllocated()) {
            if (bDrawDepth){
                depthShader.begin();
                depthShader.setUniform1f("low1", kinectLow.get());
                depthShader.setUniform1f("high1", kinectHigh.get());
                depthShader.setUniform3f("colorLow", depthColorLow.get().r, depthColorLow.get().g, depthColorLow.get().b);
                depthShader.setUniform3f("colorHigh", depthColorHigh.get().r, depthColorHigh.get().g, depthColorHigh.get().b);
                
                depthTex0.draw(bDrawColor ? 1920 : 0, 0, KINECT2_DEPTH_X, KINECT2_DEPTH_Y);
                depthShader.end();
            }
            if ( bDrawIr ){
                irShader.begin();
                irTex0.draw(bDrawColor ? 1920 + KINECT2_DEPTH_X : KINECT2_DEPTH_X, 0, KINECT2_DEPTH_X, KINECT2_DEPTH_Y);
                irShader.end();
            }
        }
        
//        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
//        ofDrawBitmapStringHighlight("Device Count : " + ofToString(ofxMultiKinectV2::getDeviceCount()), 10, 40);
    }
    
    // getters
    
    ofShader & getDepthShader(){
        return depthShader;
    }
    
    ofShader & getIrShader(){
        return irShader;
    }
    
    ofxMultiKinectV2 & getKinect(){
        return kinect0;
    }
    
    ofTexture & getColorTexture(){
        return colorTex0;
    }
    
    
    ofTexture & getDepthTexture(){
        return depthTex0;
    }
    
    ofTexture & getIrTexture(){
        return irTex0;
    }
    
    // params
    ofParameter<float> kinectLow, kinectHigh;
    ofParameter<ofFloatColor> depthColorLow, depthColorHigh;
    ofParameter<bool> bDrawColor, bDrawDepth, bDrawIr;
    
protected:
    
    
    ofxMultiKinectV2 kinect0;
    ofTexture colorTex0;
    ofTexture depthTex0;
    ofTexture irTex0;
    
    ofShader depthShader;
    ofShader irShader;
};