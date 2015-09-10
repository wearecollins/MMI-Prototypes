//
//  MovieGrid.cpp
//  MuppetShow
//
//  Created by Collins Digital Gallery on 6/11/15.
//
//

#include "MovieGrid.h"
#include "ofAVFoundationVideoPlayer.h"

    
void MovieGrid::setup( ofFbo & kinect, string directory, string fmt){
    this->kinect = &kinect;
    ofDirectory dir;
    if ( fmt.find(",") != string::npos ){
    } else {
        dir.allowExt(fmt);
    }
    
    // not drawing grids of videos, simply sending sprite sheets to shader
    bUseSpriteSheets = true;
    
    
    int n = fmin(46,dir.listDir(directory));
    
    for ( int i=0; i<n; i++){
        videos.push_back( new ofAVFoundationPlayer());
        videos.back()->loadAsync( ofToDataPath(dir.getPath(i), true) );//, OF_QTKIT_DECODE_TEXTURE_ONLY);
        videos.back()->play();
        videos.back()->setLoopState(OF_LOOP_NORMAL);
        //ofAVFoundationVideoPlayer * avf = ;
//        [((ofAVFoundationVideoPlayer*)videos.back()->getAVFoundationVideoPlayer()) setEnableAudioSampling:NO];
        //            videos.back().setPaused(true);
    }
    
    
    player = videos[ floor(ofRandom(videos.size()))];
    
    drawCount = 100;
    
    gridX = drawCount;
    gridY = 0;
    
    while( gridY < gridX && gridX > 1){
        gridX--;
        gridY++;
        if ( gridX == 1 ){//|| drawCount/gridY <= gridX ){
            //                cout << drawCount/gridX  <<":"<<gridY<<endl;
            break;
        }
    }
    
    dGridX = (float) 960. / (60.);
    dGridY = (float) videos.size() / dGridX;
    
    if ( !bUseSpriteSheets ){
        render.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
        videoFbo.allocate( 960., dGridY * 40.);
    }
    renderShader.load("", "render.frag");
    
    cout << dGridX <<":"<<dGridY<<endl;
    
//    startThread();
    bUpdateMe = false;
}

void MovieGrid::update(){
    static int updateIndex = 0;
    static int interval = 5000;
    static int last = 0;
    
    //        if ( ofGetElapsedTimeMillis() - last > interval ){
    //            interval = ofRandom(500,3000);
    //            last = ofGetElapsedTimeMillis();
    //            player->setPaused(true);
    //            player = &videos[ floor(ofRandom(videos.size()))];
    //            player->setPaused(false);
    //        }
    
    for (updateIndex; updateIndex<updateIndex+3; updateIndex++){
        videos[updateIndex]->update();
        if ( updateIndex >= videos.size()-1){
            updateIndex = 0;
            break;
        }
    }
}


void MovieGrid::threadedFunction(){
    while (isThreadRunning()) {
        if ( bUpdateMe || true ){
            for (auto * v : videos ){
                lock();
                v->update();
                unlock();
            }
            bUpdateMe = false;
        }
        yield();
    }
}

void MovieGrid::draw(){
    float w = 60.;
    float h = 40.;
    float scale = (float) (gridX * 60.)/(ofGetWidth()) ;
    int index = 0;
    //        for (int x=0; x<gridX; x++){
    //            for (int y=0; y<gridY; y++){
    //                videos[index]->update();
    //                videos[index]->draw(x * 480. * scale, y * h * scale, 480. * scale, h * scale);
    //                index++;
    //                index %= videos.size();
    //            }
    //        }
    if ( !bUseSpriteSheets ){
        videoFbo.begin();
        ofClear(0);
        for (int x=0; x<dGridX; x++){
            for (int y=0; y<dGridY; y++){
                int index = x + (y * dGridX);
                if ( index > videos.size()){
                    break;
                }
                videos[index]->draw(x * w, y * h, w, h);
            }
        }
        bUpdateMe = true;
        videoFbo.end();
    }
    
    renderShader.begin();
    if ( !bUseSpriteSheets ){
        renderShader.setUniformTexture("tex0", videoFbo.getTexture(0), 1);
    } else {
        renderShader.setUniformTexture("tex0", *videos[0]->getTexturePtr(), 1);
    }
    renderShader.setUniformTexture("kinect", kinect->getTexture(0), 2);
    renderShader.setUniform1i("gridX", gridX);
    renderShader.setUniform1i("gridY", gridY);
    renderShader.setUniform1i("videoGridX", dGridX);
    renderShader.setUniform1i("videoGridY", dGridY);
    renderShader.setUniform1i("numVideos", videos.size());
    renderShader.setUniform1i("videoWidth", w);
    renderShader.setUniform1i("videoHeight", h);
    renderShader.setUniform1f("kinectScale", (float) ofGetWidth() / 640.f);
    //        renderShader.setUniform1f("scale", scale);
    
    render.draw(0,0);
    renderShader.end();
    
    //        videoFbo.draw(0,0);
}

void MovieGrid::reload(){
    renderShader.unload();
    renderShader.load("", "render.frag");
}

void MovieGrid::resize(){
    render.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
}
