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
//    if ( fmt.find(",") != string::npos ){
//    } else {
//        dir.allowExt(fmt);
//    }
    
    // not drawing grids of videos, simply sending sprite sheets to shader
    bUseSpriteSheets = true;
    dir.allowExt("mov");
    dir.allowExt("mp4");
    int n = fmin(46,dir.listDir(ofToDataPath(directory,true)));
//    cout<<n<<endl;
    for ( int i=0; i<n; i++){
        videos.push_back( ofPtr<ofAVFoundationPlayer>(new ofAVFoundationPlayer()) );
        videos.back()->load( ofToDataPath(dir.getPath(i), true) );//, OF_QTKIT_DECODE_TEXTURE_ONLY);
        videos.back()->setLoopState(OF_LOOP_NORMAL);
        videos.back()->play();
        videos.back()->setVolume(0.0);
        //[videos.back()->getAVFoundationVideoPlayer() setEnableAudioSampling:NO];
        //ofAVFoundationVideoPlayer * avf = ;
//        [((ofAVFoundationVideoPlayer*)videos.back()->getAVFoundationVideoPlayer()) setEnableAudioSampling:NO];
        //            videos.back().setPaused(true);
    }
    
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
    
    render.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    renderFinal.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    
    if ( !bUseSpriteSheets ){
        videoFbo.allocate( 960., dGridY * 40.);
    }
    
    if ( ofIsGLProgrammableRenderer() ){
        renderShader.load("passthru.vert", "render_pr.frag");
    } else {
        renderShader.load("", "render.frag");
    }
    
    
    // setup activation texture
    activeData = new float[10 * 12 ];
    for (int y=0; y<10;y++){
        for ( int x=0; x<12;x++){
            activeData[y * 12 + x] = 0.;
        }
    }
    activationTexture.allocate(12, 10, GL_RGB);
    activationTexture.getTexture().setTextureMinMagFilter(GL_LINEAR,GL_NEAREST);
    activationTexture.getTexture().loadData(activeData, 12, 10, GL_LUMINANCE);
    
    for (int i=0; i<120; i++){
        active.push_back(false);
    }
    
//    setActive(12);
    
//    cout << dGridX <<":"<<dGridY<<endl;
    
//    startThread();
    bUpdateMe = false;
}

int rate = 1000;
uint64_t lastChanged = 0;

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
    
//    for (updateIndex; updateIndex<updateIndex+3; updateIndex++){
//        videos[updateIndex]->update();
//        if ( updateIndex >= videos.size()-1){
//            updateIndex = 0;
//            break;
//        }
//    }
    
//    if ( ofGetElapsedTimeMillis() - lastChanged > rate ){
//        rate = ofRandom(2000,5000);
//        lastChanged = ofGetElapsedTimeMillis();
//        
//        for ( int i=0; i<active.size(); i++ ){
//            active[i] = false;
//        }
//        
//        for ( int i=0; i<active.size(); i++ ){
//            if ( ofRandomf() > .7 )
//                active[i] = true;
//        }
//    }
//    
//    for (int i=0; i<active.size(); i++){
//        float target = 0.;
//        
//        if ( active[i] ) target = 1.;
//        
//        activeData[ i ] = activeData[ i ] * .9 +  target * .1;
//    }
    
    for (auto & v : videos ){
//        lock();
        v.get()->update();
        v.get()->setVolume(0.0);
//        unlock();
    }
    
//    activationTexture.getTexture().loadData(activeData, 12, 10, GL_LUMINANCE);
}


void MovieGrid::threadedFunction(){
    while (isThreadRunning()) {
        if ( bUpdateMe || true ){
            for (auto & v : videos ){
                lock();
                v.get()->update();
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
    
    renderFinal.begin();
    renderShader.begin();
    if ( !bUseSpriteSheets ){
        renderShader.setUniformTexture("tex0", videoFbo.getTexture(0), 1);
        renderShader.setUniformTexture("kinect", kinect->getTexture(0), 2);
        renderShader.setUniform1i("gridX", gridX);
        renderShader.setUniform1i("gridY", gridY);
        renderShader.setUniform1i("videoGridX", dGridX);
        renderShader.setUniform1i("videoGridY", dGridY);
        renderShader.setUniform1i("numVideos", videos.size());
        renderShader.setUniform1i("videoWidth", w);
        renderShader.setUniform1i("videoHeight", h);
        renderShader.setUniform1f("kinectScale", (float) ofGetWidth() / 640.f);
    } else {
        renderShader.setUniformTexture("tex0", *videos[0]->getTexturePtr(), 2);
        renderShader.setUniformTexture("tex1", *videos[1]->getTexturePtr(), 3);
        renderShader.setUniformTexture("tex2", *videos[2]->getTexturePtr(), 4);
        
        renderShader.setUniformTexture("kinect", kinect->getTexture(0), 5);
        renderShader.setUniformTexture("activationTexture", activationTexture.getTexture(), 6);
        
        renderShader.setUniform1f("kinectScale", (float) ofGetWidth() / 640.f);
        
        renderShader.setUniform1f("time", ofGetElapsedTimef());
        renderShader.setUniform1i("numVideos", 120);
        renderShader.setUniform1i("videoWidth", 160);
        renderShader.setUniform1i("videoHeight", 120);
        renderShader.setUniform1i("screenWidth", ofGetWidth());
        renderShader.setUniform1i("screenHeight", ofGetHeight());
    }
    
    render.draw(0,0);
    renderShader.end();
    renderFinal.end();
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    ofScale(1, 1);
    ofTranslate(-ofGetWidth()/2.0, -ofGetHeight()/2.0);
    renderFinal.draw(0,0);
    ofPopMatrix();
    
    if ( ofGetKeyPressed('Y')){
        activationTexture.draw(0,0, 12 * 75, 10 * 75);
    }
    
    //        videoFbo.draw(0,0);
}

//---------------------------------------------------------------
void MovieGrid::reload(){
    renderShader.unload();
    renderShader.load("passthru.vert", "render_pr.frag");
}

//---------------------------------------------------------------
void MovieGrid::resize(){
    render.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    renderFinal.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
}

//---------------------------------------------------------------
void MovieGrid::setActive ( int index ){
    if ( index < 120 ){
        activeData[ index ] = 1.;
    } else {
        ofLogWarning()<<"Index exceeds # pixels";
    }
}

//---------------------------------------------------------------
void MovieGrid::setInactive( int index ){
    if ( index < 120 ){
        activeData[ index ] = 0.;
    } else {
        ofLogWarning()<<"Index exceeds # pixels";
    }
}

//---------------------------------------------------------------
void MovieGrid::rewind(){
    
    for (auto & v : videos ){
        v->setPosition(0.);
    }
}
