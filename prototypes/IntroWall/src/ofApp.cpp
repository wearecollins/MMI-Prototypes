#include "ofApp.h"

bool bDrawGui = true;

float currentMouse = 0.;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    panel.setup();
    meshSettings.setName("Mesh");
    meshSettings.add(x.set("x", 0, 0, ofGetWidth()));
    meshSettings.add(y.set("y", 0, 0, ofGetHeight()));
    meshSettings.add(w.set("w", 100, 0, ofGetWidth()));
    meshSettings.add(h.set("h", 300, 0, ofGetWidth()));
    meshSettings.add(step.set("step", 10, 1, 100));
    meshSettings.add(space.set("spacing", 0, 0., 1000));
    meshSettings.add(drawMode.set("drawMode", 0, 0, 2));
    meshSettings.add(primMode.set("primitiveMode", 0, 0, 11));
    meshSettings.add(distanceThresh.set("distanceThresh", 0, 0, 500.));
    meshSettings.add(mouseRadius.set("mouseRadius", 0, 0, 500.));
    panel.add(meshSettings);
    
    panel.loadFromFile("settings.xml");

}

//--------------------------------------------------------------
void ofApp::update(){
    mesh.clear();
    
    while (w % step != 0 ){
        w--;
    }
    
    float t = ofGetElapsedTimeMillis();
    
    float spX = w == 0 ? 0 : (float) space / w;
    float spY = h == 0 ? 0 : (float) space / h;
    
    bool bLeft = false;
    bool bBottom = false;
    
    ofVec2f m(mouseX, mouseY);
    
    for ( int _y =0; _y<h; _y+=step){
        for ( int _x =0; _x<w; _x+=step){
            ofVec3f v(_x + x + (bLeft ? space.get() : 0), _y + y + (bBottom ? space.get() : 0), 0.);
            float d = v.distance(m);
            float z =  d > distanceThresh ? 0 : ofMap(d, 0., distanceThresh, 0., mouseRadius);
            v.z = z;
            mesh.addVertex(v);
            mesh.addTexCoord(ofVec2f(_x, _y));
            bLeft = !bLeft;
        }
        bBottom = !bBottom;
    }
    
    static vector<ofIndexType> baseIndices;
    baseIndices.clear();
    int _x = 0;
    int _y = 0;
    
    int gw = ceil(w / step);
    int mw = gw*step;
    int mh = h;
    
    for (float ystep = 0; ystep < mh-step; ystep += step){
        for (float xstep = 0; xstep < mw-step; xstep += step){
            ofIndexType a,b,c;
            a = _x+_y*gw;
            b = (_x+1)+_y*gw;
            c = _x+(_y+1)*gw;
            
            if ( primMode == 1 || primMode == 2 ){
                baseIndices.push_back(a);
                baseIndices.push_back(b);
                baseIndices.push_back(c);
            }
            if ( primMode == 0 || primMode >= 2 ){
                
                a = (_x+1)+(_y+1)*gw;
                b = _x+(_y+1)*gw;
                c = (_x+1)+(_y)*gw;
                baseIndices.push_back(a);
                baseIndices.push_back(b);
                baseIndices.push_back(c);
            }
            _x++;
        }
        
        _y++;
        _x = 0;
    }
    
    mesh.addIndices(baseIndices);
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
//    mesh.setMode((ofPrimitiveMode) primMode.get());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();

    switch (drawMode) {
        case 0:
            mesh.draw();
            break;
            
        case 1:
            mesh.drawWireframe();
            break;
            
        default:
            mesh.drawVertices();
            break;
    }
    
    ofDisableDepthTest();
    
    if ( bDrawGui )
        panel.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'g' ){
        bDrawGui = !bDrawGui;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
