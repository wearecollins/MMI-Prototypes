//
//  Kaleidoscope.h
//  MMI_Kaleidoscope
//
//  Created by Brett Renfer on 10/6/15.
//
//

#pragma once
#include "Constants.h"

class Kaleidoscope
{
    public :
    
    float gDrawWidth, gDrawHeight;
    float count; /// point speed of rotation
    float spinStep; // speed of the step of each rotation///
    float triangleCameraGrab;
    int imgLifeCap;
    float ksx;
    float ksy;
    
    float  u1, v1, u2, v2, u3, v3;
    float  u4, v4, u5, v5, u6, v6;
    float vw, vh;
    
    //texture coords
    ofPoint texCoord1;
    ofPoint texCoord2;
    ofPoint texCoord3;
    
    ofPoint centerPoint;
    ofPoint centerPoint2;
    
    bool bDrawDebug;
    bool bDragging[2];
    
    float radius[2];
    float offSet[2];
    
    Kaleidoscope(){};
    
    void setup( float _gDrawWidth, float _gDrawHeight, float _spinStep){
        count= 0.0f; /// point speed of rotation
        spinStep = _spinStep; // speed of the step of each rotation///
        triangleCameraGrab = 1000;
        imgLifeCap 	= 900;
        ksx=300;//150;
        ksy=300;//150;
        
        gDrawWidth = _gDrawWidth;
        gDrawHeight = _gDrawHeight;
        bDrawDebug = false;
        centerPoint.x = 0.0;
        centerPoint.y = 0.0;
        centerPoint2.x = 0.0;
        centerPoint2.y = 0.0;
        
        for (int i=0; i<2; i++){
            radius[i] = 0;
            offSet[i] = 0;
        }
    };
    
    void toggeleDrawDebug(){
        bDrawDebug = !bDrawDebug;
    }
    
    void update(){
        // this controls the speed of triangle rotation
        count  -= spinStep;
        // make decrement variable...
        
        
        // these control the size of the camer grab/////////////////////////////////////////////////////////////////////////
        //tr1 = tr2 = tr3 = triangleCameraGrab;
        
        radius[0] = triangleCameraGrab - offSet[0];
        radius[1] = triangleCameraGrab - offSet[1];
        
        //		if (radius[0] > 350) radius[0] = 350;
        //		if (radius[1] > 350) radius[1] = 350;
        
        bDrawDebug = false;
    }
    
    void render(float _gDrawWidth, float _gDrawHeight, ofTexture& textureImg){
        gDrawWidth = _gDrawWidth;
        gDrawHeight = _gDrawHeight;
        
        // println("My texture is "+textureImg);
        float cc = 70;
        float ix,iy;
        
        /*int vw = gVideoWidth;
         int vh = gVideoHeight;*/
        
        vw = textureImg.getWidth();
        vh = textureImg.getHeight();
        float vw10 = vw / 10.0f;
        float vh10 = vh / 10.0f;
        float vw2 = vw / 2.0f;
        float vh2 = vh / 2.0f;
        float mmm = 100000.0;
        
        //change direction of 'spinStep' when hitting maximum float value in 'count'
        //to keep kaleidoscope rotating
        if (count<-4194303. || count>4194303.) {
            spinStep=-spinStep;
        }
        
        //  vw2 = (int) ((float) (millis() % mmm) * (float) vw / mmm);
        int maxWidth = 1280;//320;
        int maxHeight = 1024;//240;
        u1 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+0)*radius[0]), maxWidth-centerPoint.x));
        v1 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+0)*radius[0]), maxHeight-centerPoint.y));
        u2 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f)*radius[0]), maxWidth-centerPoint.x));
        v2 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f)*radius[0]), maxHeight-centerPoint.y));
        u3 = fmax(0, centerPoint.x + fmin(vw2 + (sin(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxWidth-centerPoint.x));
        v3 = fmax(0, centerPoint.y + fmin(vh2 + (cos(count/cc+ PI *2.f/3.f*2.f)*radius[0]), maxHeight-centerPoint.y));
        //		cout<<u1<<" "<<v1<<" "<<u2<<" "<<v2<<" "<<u3<<" "<<v3<<endl;
        int numI = 2;
        
        for(iy=0;iy<(gDrawHeight/ksy)+1;) {
            
            for(int i = 0; i < numI; i++, iy++){
                textureImg.bind();
                
                glBegin(GL_TRIANGLE_STRIP);
                //textureImg.blend(textureImg,0,0,640,480,0,0,640,480,ADD);
                glTexCoord2f(u2, v2);
                glVertex2f(ix * ksx- ksx/2, (iy+1)*ksy);
                for(ix = 0 ; ix < (gDrawWidth / ksx) + 1 ; ix++) {
                    glTexCoord2f(u1, v1);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u3, v3);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                    ix++;
                    glTexCoord2f(u2,v2);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u1, v1);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                    
                    ix++;
                    glTexCoord2f(u3,v3);
                    glVertex2f(ix * ksx, (iy + i) * ksy);
                    glTexCoord2f(u2, v2);
                    glVertex2f((ix+1)* ksx- ksx/2, (iy+numI-i-1)*ksy);
                }
                glEnd();
                
                textureImg.unbind();
            }
        }
    }
    
    void render(float _gDrawWidth, float _gDrawHeight, ofImage textureImg) {
        render(_gDrawWidth, _gDrawHeight, textureImg.getTextureReference());
    }
    
    void renderDebug( ofImage textureImg){
        textureImg.draw(0,0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u1, v1);
        ofVertex(u2, v2);
        ofVertex(u3, v3);
        ofEndShape(true);
        ofPopStyle();
        
        bDrawDebug = true;
    }
    
    void renderDebug( ofImage textureImg, ofImage textureImg2 ){
        textureImg.draw(0,0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u1, v1);
        ofVertex(u2, v2);
        ofVertex(u3, v3);
        ofEndShape(true);
        ofPopStyle();
        
        textureImg2.draw(textureImg.width,0);
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        ofVertex(u4 + vw, v4 );
        ofVertex(u5 + vw, v5 );
        ofVertex(u6 + vw, v6 );
        ofEndShape(true);
        ofPopStyle();
        
        bDrawDebug = true;
    }
    
    void mousePressed( int x, int y){
        bDragging[0] = false;
        bDragging[1] = false;
        
        if ( bDrawDebug ){
            if (x < vw && y < vh){
                bDragging[0] = true;
                centerPoint.x = x - vw/2.0f;
                centerPoint.y = y - vh/2.0f;
            } else if ( x < vw*2 && y < vh ){				
                bDragging[1] = true;
                centerPoint2.x = x - vw - vw/2.0f;
                centerPoint2.y = y - vh/2.0f;
            }
        }
    };
    
    void mouseDragged( int x, int y){
        if ( bDrawDebug ){
            if (bDragging[0]){
                centerPoint.x = x - vw/2.0f;
                centerPoint.y = y - vh/2.0f;
            } else if ( bDragging[1] ){
                centerPoint2.x = x - vw - vw/2.0f;
                centerPoint2.y = y - vh/2.0f;
            }
        }
    };
    
    void mouseReleased(){
        bDragging[0] = false;
        bDragging[1] = false;
    }
    
};