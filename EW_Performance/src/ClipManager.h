//
//  ClipManager.h
//  EW_Performance
//
//  Created by Brett Renfer on 7/8/15.
//
//

#pragma once

#include "ofMain.h"

class Clip : public ofRectangle {
public:
    
    bool setup( string path ){
        movie.load(path);
		movie.setVolume(0);
        width = movie.getWidth();
        height = movie.getHeight();
        bOver = false;
        return true;
    }
    
    void draw(){
        movie.update();
        ofPushMatrix();
        ofTranslate(this->x, this->y);
        movie.draw(0,0, this->width, this->height);
        ofPopMatrix();
    }
    
    void draw( float x, float y, float w=0, float h=0 ){
        movie.update();
        if ( w == 0 ) w = this->width;
        if ( h == 0 ) h = this->height;
        movie.draw(x,y,w,h);
        if ( bOver ){
            ofSetColor(255,100);
            ofDrawRectangle(x,y,w,h);
            ofSetColor(255);
        }
    }
    
    ofVideoPlayer & v(){
        return movie;
    }
    
    bool bOver;
    
protected:
    
    ofVideoPlayer movie;
};

enum Mode {
    MODE_SELECT = 0,
    MODE_PREVIEW,
    MODE_GETREADY,
    MODE_PERFORM,
    MODE_WATCH
};

class ClipManager {
public:
    
    // public gui-able vars
    ofParameter<float> COUNTDOWN_PREVIEW;
	ofParameter<float> COUNTDOWN_GETREADY;
	ofParameter<float> COUNTDOWN_PERFORM;
	ofParameter<float> COUNTDOWN_WATCH;
	ofParameter<bool> doPreview, doPractice;
    
	ofParameter<float> GET_READY_ALPHA;
    
    void setup( string dir = "videos" ){
		COUNTDOWN_PREVIEW.set("COUNTDOWN_PREVIEW", 15.f, 0., 30.);
		COUNTDOWN_GETREADY.set("COUNTDOWN_GETREADY", 10.f, 0., 30.);
		COUNTDOWN_PERFORM.set("COUNTDOWN_PERFORM", 15.f, 0., 30.);
		COUNTDOWN_WATCH.set("COUNTDOWN_WATCH", 15.f, 0., 30.);
		GET_READY_ALPHA.set("COUNTDOWN_PREVIEW", 100.f, 0., 255.);
		doPreview.set("Do clip preview", false);
		doPractice.set("Do practice", true);
        
        ofDirectory dirlister(ofToDataPath(dir));
        dirlister.listDir();
        
        currentClip = NULL;
        
        for ( auto & f : dirlister.getFiles() ){
            clips.push_back( Clip());
			cout << f.getAbsolutePath() << endl;
            clips.back().setup( f.getAbsolutePath() );
            clips.back().v().play();
            clips.back().v().setFrame(500);
            clips.back().v().setPaused(true);
        }
        
        if ( clips.size() != 0 ){
            currentClip = &clips[0];
        }
        
        setMode(MODE_SELECT);
        ofAddListener(ofEvents().mousePressed, this, &ClipManager::onMousePressed);
        // de hg
//        ofAddListener(ofEvents().mouseMoved, this, &ClipManager::onMouseMoved);
        
        bool fontLoaded = font.load("fonts/DIN.otf", 300);
        fontSmall.load("fonts/DIN.otf", 100);
        fontSmall.setLineHeight(110.);
        
        cout <<"FONT: "<<fontLoaded<<endl;
    }
    
    float getCurrentCountdown(){
        float ret = 0.;
        switch (currentMode ){
            case MODE_PREVIEW:
                ret = fmax(0,(COUNTDOWN_PREVIEW - (ofGetElapsedTimef() - time)));
                break;
                
            case MODE_PERFORM:
                ret =  fmax(0,(COUNTDOWN_PERFORM - (ofGetElapsedTimef() - time)));
                break;
                
            case MODE_GETREADY:
                ret =  fmax(0,(COUNTDOWN_GETREADY - (ofGetElapsedTimef() - time)));
                break;
        }
        return ret;
    }
    
    void update(){
        switch (currentMode ){
            case MODE_SELECT:
                break;
            case MODE_PREVIEW:
                if ( ofGetElapsedTimef() - time >= COUNTDOWN_PREVIEW ){
                    setMode(doPractice ? MODE_GETREADY : MODE_PERFORM);
                }
                break;
                
            case MODE_GETREADY:
                if ( ofGetElapsedTimef() - time >= COUNTDOWN_GETREADY ){
                    setMode(MODE_PERFORM);
                }
                break;
                
            case MODE_PERFORM:
                if ( ofGetElapsedTimef() - time >= COUNTDOWN_PERFORM ){
                    setMode(MODE_WATCH);
                }
                break;
                
            case MODE_WATCH:
                if ( ofGetElapsedTimef() - time >= COUNTDOWN_WATCH ){
                    setMode(MODE_SELECT);
                }
                break;
        }
        
    }
    
    void draw(){
        update();
        switch (currentMode ){
            case MODE_SELECT:
            {
                float x = 0;
                float y = 0;
                float sp = 50;
				ofLogError() << "mute" << endl;
                for ( auto & c : clips ){
					c.v().setVolume(0);
                    c.draw(x,y);
                    y += c.v().getHeight();
                    y += sp;
                }
            }
                break;
            case MODE_PREVIEW:
                if ( currentClip != NULL ){
                    currentClip->draw();
                }
                break;
                
            case MODE_PERFORM:
                
                break;
        }
        
    }
    
    void draw( float x, float y, float w, float h ){
        update();
        switch (currentMode ){
            case MODE_SELECT:
            {
                float cw = 0;
                float ch = 0;
                float sp = 50;
                static int bSet = 0;
                
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = w/2. - fw/2.;
                float fy = 50 + fontSmall.getSize();
                ofSetColor(255);
                fontSmall.drawString( "Select a clip\nto begin", x + w/2. - fontSmall.stringWidth( "Select a clip\nto begin!" )/2., fy);
                
                y += fy; + fontSmall.getLineHeight()/2.;
                
                for ( auto & c : clips ){
					c.v().setVolume(0);
                    
//                    if ( bSet < 30 ){
//                        c.v().setPosition(.5);
//                        c.v().update();
//                        bSet++;
//                    }
                    c.v().update();
                    if ( cw == 0 ){
                        float s = h/ ((c.v().getHeight() * clips.size()) + (sp * clips.size()));
                        ch = c.v().getHeight() * s;
                        cw = c.v().getWidth() * s;
                        x = x + (w/2.0) - cw/2.0;
                    }
                    static bool bLogged = false;
                    if (!bLogged){
//                        bLogged = true;
                    }
                    c.draw(x,y,cw,ch);
                    c.set(x,y,cw,ch);
                    y += ch;
                    y += sp;
                }
            }
                break;
            case MODE_PREVIEW:
            {
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - font.getSize()/2.0;// + font.getSize();
                
                ofSetColor(255);
                fontSmall.drawString( "Preview", x + w/2. - fontSmall.stringWidth( "Preview" )/2., fy);
                fy += font.getSize() * 1.5;
                font.drawString( ofToString(getCurrentCountdown(),0), fx, fy);
            }
                break;
                
            case MODE_GETREADY:
            {
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - font.getSize()/2.0;// + font.getSize();
                ofSetColor(255);
                fontSmall.drawString( "Get Ready!", x + w/2. - fontSmall.stringWidth( "Get Ready!" )/2., fy);
                fy += font.getSize() * 1.5;
                font.drawString( ofToString(getCurrentCountdown(),0), fx, fy);
            }
                break;
                
            case MODE_PERFORM:
            {
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - font.getSize()/2.0;// + font.getSize();
                ofSetColor(255);
                fontSmall.drawString( "Perform!", x + w/2. - fontSmall.stringWidth( "Perform!" )/2., fy);
                fy += font.getSize() * 1.5;
                font.drawString( ofToString(getCurrentCountdown(),0), fx, fy);
            }
                break;
            case MODE_WATCH:
                float fw = font.stringWidth( ofToString(10,0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - font.getSize();// + font.getSize();
                ofSetColor(255);
                fontSmall.drawString( "Share your \nperformance!", x + w/2. - fontSmall.stringWidth( "Share your \nperformance!" )/2., fy);
                break;
        }
        
    }
    
    void drawPreview(float x, float y, float w, float h ){
        switch (currentMode ){
            case MODE_SELECT:
                break;
            case MODE_PREVIEW:
                if ( currentClip != NULL ){
                    float s = fmin( w/currentClip->v().getWidth(), h/currentClip->v().getHeight());
                    x = x + w/2.0 - ( currentClip->v().getWidth() * s/2.0 );
                    
                    ofSetColor( 255, ofMap( getCurrentCountdown(), 3, 0, 255, 0, true ) );
                    
                    currentClip->v().setVolume( fmin( 1, ofMap( getCurrentCountdown(), 3, 0, 1, 0, true )));
                    
                    currentClip->draw(x,y,currentClip->v().getWidth() * s,currentClip->v().getHeight() * s);
                }
                break;
                
            case MODE_PERFORM:
                break;
            case MODE_WATCH:
                break;
            case MODE_GETREADY:
                if ( currentClip != NULL ){
                    float s = fmin( w/currentClip->v().getWidth(), h/currentClip->v().getHeight());
                    x = x + w/2.0 - ( currentClip->v().getWidth() * s/2.0 );
                    
                    ofSetColor( 255, ofMap( getCurrentCountdown(), 3, 0, GET_READY_ALPHA, 0, true ) );
                    
                    currentClip->v().setVolume( fmin( 1, ofMap( getCurrentCountdown(), 3, 0, 1, 0, true )));
                    currentClip->draw(x,y,currentClip->v().getWidth() * s,currentClip->v().getHeight() * s);
                }
                break;
        }
    }
    
    void setMode( Mode m ){
        currentMode = m;
        
        switch ( currentMode ){
            case MODE_SELECT:
                for ( auto & c : clips ){
                    c.v().play();
                    c.v().setVolume(0);
                }
                break;
                
            case MODE_PREVIEW:
                if ( currentClip != NULL ){
                    currentClip->v().play();
					currentClip->v().setPosition(ofRandom(.2, .7));
                    currentClip->v().setVolume(1);
//                    currentClip->v().setPosition(0);
                }
                break;
                
            case MODE_GETREADY:
                if ( currentClip != NULL ){
                    //                    currentClip->v().setPaused(true);
                    currentClip->v().setVolume(1);
                    currentClip->v().setPosition(0);
                }
                time = ofGetElapsedTimef();
                break;
            
            case MODE_PERFORM:
                if ( currentClip != NULL ){
                    currentClip->v().play();
                    currentClip->v().setPosition(0);
                    currentClip->v().setVolume(1);
                }
                time = ofGetElapsedTimef();
                break;
                
            case MODE_WATCH:
                if ( currentClip != NULL ){
                    currentClip->v().setPaused(true);
                }
                time = ofGetElapsedTimef();
                break;
        }
    }
    
    // for mouse
    void setPos( float x, float y, float s){
        mPos.x = x;
        mPos.y = y;
        mPos.z = s;
    }
    
    void onMousePressed( ofMouseEventArgs & e ){
        if ( currentMode == MODE_WATCH ){
            
            ofVec2f m(e);
            m -= ofVec2f(mPos.x, mPos.y);
            if ( m.x > 0 ){
                setMode(MODE_SELECT);
                return;
            }
        }
        if ( currentMode != MODE_SELECT ) return;
        ofVec2f m(e);
        m -= ofVec2f(mPos.x, mPos.y);
        m *= 1./mPos.z;
        
        for ( auto & c : clips ){
            if ( c.inside(m)){
                currentClip = &c;
				currentClip->v().setVolume(1.);
                setMode(doPreview ? MODE_PREVIEW : ( doPractice ? MODE_GETREADY : MODE_PERFORM ) );
                time = ofGetElapsedTimef();
                break;
            }
        }
    }
    
    void onMouseMoved( ofMouseEventArgs & e ){
        if ( currentMode != MODE_SELECT ) return;
        ofVec2f m(e);
        m -= ofVec2f(mPos.x, mPos.y);
        m *= 1./mPos.z;
        
        for ( auto & c : clips ){
            if ( c.inside(m)){
                c.bOver = true;
                break;
            } else {
                c.bOver = false;
            }
        }
    }
    
    Mode getMode(){ return currentMode; };
    
protected:
    Mode currentMode;
    vector<Clip> clips;
    Clip * currentClip;
    ofVec3f mPos;
    
    // timer for preview
    
    ofTrueTypeFont font, fontSmall;
    float time;
};