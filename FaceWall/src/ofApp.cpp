#include "ofApp.h"

#include "ofAppGLFWWindow.h"

bool debug = false;
ofImage last, curr;
float pixelScale = 1.0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetCircleResolution(100);
    ofSetWindowPosition(0, 0);
    
    ofAppGLFWWindow* window = (ofAppGLFWWindow*)(ofGetWindowPtr());
    pixelScale = window->getPixelScreenCoordScale();
    
    EYE_WIDTH *= pixelScale;
    NOSE_SCALE *= pixelScale;
    
    ofSetWindowShape(1920 * pixelScale,1080 * pixelScale);
    sampler.load(ofToDataPath("colors.svg"));
    float w = 200. * pixelScale;
    float h = 200. * pixelScale;
    for ( float x = w; x<ofGetWidth() - w * 2; x+=w*1.1){
        for ( float y = h;y<ofGetHeight() - w * 2; y+=h*1.1){
//            w  = ofRandom(50,200);
//            h = ofRandom(50,100);
            faces.push_back(Face());
            faces.back().set(x, y, w, h);
            faces.back().color = sampler.getRandomColor();
        }
    }
    
    camera.initGrabber(160,120);
    flow.calcOpticalFlow(camera);
    flow.setNumLevels(1);
}
ofVec2f pos(0,0);

//--------------------------------------------------------------
void ofApp::update(){
    camera.update();
    
    if ( camera.isFrameNew() ){
        static ofImage copy;
        copy.setFromPixels(camera.getPixelsRef());
        copy.setImageType(OF_IMAGE_GRAYSCALE);
        copy.mirror(false, true);
        
        flow.calcOpticalFlow(copy);
        int w = flow.getWidth();
        int h = flow.getHeight();
        
        float max = 2.0;
        float scaleW = (float) (ofGetWidth()) / (float) camera.getWidth();
        float scaleH = (float) (ofGetHeight()) / (float) camera.getHeight();
        
        for (int x=0; x<w; x+=2){
            for (int y=0; y<h; y+=2){
                float mx = w-x;
                ofVec2f t = flow.getFlowPosition(mx, y);
                
                if ( fabs(t.x-mx) > max || fabs(t.y-y) > max ){
                    for ( auto & f : faces ){
                        if ( f.inside(x * scaleW, y * scaleH) && !f.bActive){
                            f.bActive =  true;
                        }
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( auto & f : faces ){
        f.draw();
    }
    
    if (debug ){
    //    ofxCv::drawMat(flow.getFlow(), 0, 0);
        
        float scaleW = (float) (ofGetWidth()) / (float) camera.getWidth();
        float scaleH = (float) (ofGetHeight()) / (float) camera.getHeight();
        ofScale(scaleW * -1, scaleH);
        ofTranslate(-flow.getWidth(), 0);
        flow.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'r' ){
        
        for ( auto & f : faces ){
            f.bReload = true;
        }
    } else if ( key == 'a' ){
        
        for ( auto & f : faces ){
            if ( ofRandomf() > .5 ){
                f.bActive = !f.bActive;
            }
        }
    } else if ( key == 'd'){
        debug = !debug;
    } else if ( key == 'f' ){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for ( auto & f : faces ){
        if ( f.inside(x, y)){
            f.bActive = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
