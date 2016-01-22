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
    MODE_WATCH,
    MODE_SHARE
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
			clips.back().v().setVolume(0);
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
        
        bool fontLoaded = font.load("fonts/DIN.otf", 200);
        fontSmall.load("fonts/DIN.otf", 50);
        fontSmall.setLineHeight(60.);
        
        // new interface overlayz
        string i_p  = "images/overlay_";
        oSelect.load( i_p + "select.png");
        oPractice.load( i_p + "practice.png");
        oPerform.load( i_p + "perform.png");
        oShare.load( i_p + "share.png");
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
    
    float getCurrentCountdownPct(){
        float ret = 0.;
        switch (currentMode ){
            case MODE_PREVIEW:
                ret = fmax(0,(ofGetElapsedTimef() - time) / (COUNTDOWN_PREVIEW));
                break;
                
            case MODE_PERFORM:
                ret = fmax(0,(ofGetElapsedTimef() - time) / (COUNTDOWN_PERFORM));
                break;
                
            case MODE_GETREADY:
                ret = fmax(0,(ofGetElapsedTimef() - time) / (COUNTDOWN_GETREADY));
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
                break;
                
            case MODE_SHARE:
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
                for ( auto & c : clips ){
					c.v().setVolume(0);
                    c.draw(x,y);
                    x += c.v().getWidth();
                    x += sp;
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
    
    void draw(bool bVert, float x, float y, float w, float h ){
        update();
        if ( bVert ){
            drawVertical(x,y,w,h);
        } else {
            drawHoriz(x,y,w,h);
        }
    }
    
    void drawPreview(bool bVert, float x, float y, float w, float h ){
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
        
		static float pos = ofRandom(.2, .7);

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
					pos = ofRandom(.2, .7);
                    //                    currentClip->v().setPaused(true);
                    currentClip->v().setVolume(1);
					currentClip->v().setPosition(pos);
                }
                time = ofGetElapsedTimef();
                break;
            
            case MODE_PERFORM:
                if ( currentClip != NULL ){
                    currentClip->v().play();
					currentClip->v().setPosition(pos);
                    currentClip->v().setVolume(1);
                }
                time = ofGetElapsedTimef();
                break;
                
            case MODE_WATCH:
                if ( currentClip != NULL ){
                    currentClip->v().setPaused(true);
					currentClip->v().play();
					currentClip->v().setPosition(pos);
					currentClip->v().setVolume(1);
                }
                time = ofGetElapsedTimef();
                break;
            
            case MODE_SHARE:
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
    
    // images
    ofImage oSelect, oPractice, oPerform, oShare;
    
    // draw
    void drawVertical( float x, float y, float w, float h ){
        bool bVert = true;
        switch (currentMode ){
            case MODE_SELECT:
            {
                float cw = 0;
                float ch = 0;
                float sp = 50;
                static int bSet = 0;
                
                string s = bVert ? "Select a clip\nto begin" : "Select a clip to begin";
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = bVert ? x + w / 2. - fontSmall.stringWidth(s) / 2. : x + 20;
                float fy = 40 + fontSmall.getLineHeight() / 2.;
                ofSetColor(255);
                fontSmall.drawString( s, fx, fy);
                
                if ( !bVert) y -= 50; //+ fontSmall.getLineHeight()/2.;
                
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
                        if (!bVert) {
                            s = (h - 100) / c.v().getHeight();
                        }
                        ch = c.v().getHeight() * s;
                        cw = c.v().getWidth() * s;
                        x = bVert ? x + (w/2.0) - cw/2.0 : x + 20;
                    }
                    static bool bLogged = false;
                    if (!bLogged){
                        //                        bLogged = true;
                    }
                    c.draw(x,y,cw,ch);
                    c.set(x,y,cw,ch);
                    if (bVert) {
                        y += ch;
                        y += sp;
                        
                    }
                    else {
                        x += cw;
                        x += sp;
                    }
                }
            }
                break;
            case MODE_PREVIEW:
            {
                if (bVert) {
                    float fw = font.stringWidth(ofToString(getCurrentCountdown(), 0));
                    float fx = x + w / 2. - fw / 2.;
                    float fy = h / 2.0 - font.getSize() / 2.0;// + font.getSize();
                    
                    ofSetColor(255);
                    fontSmall.drawString("Preview", x + w / 2. - fontSmall.stringWidth("Preview") / 2., fy);
                    fy += font.getSize() * 1.5;
                    font.drawString(ofToString(getCurrentCountdown(), 0), fx, fy);
                }
                else {
                    float fw = font.stringWidth(ofToString(getCurrentCountdown(), 0));
                    float fx = x + w / 2. - fw / 2.;
                    float fy = h / 2.0 - fontSmall.getSize() + 20;// + font.getSize();
                    
                    ofSetColor(255);
                    fontSmall.drawString("Preview", x + w / 2. - fontSmall.stringWidth("Preview") / 2., fy);
                }
            }
                break;
                
            case MODE_GETREADY:
            {
                if (bVert) {
                    float fw = font.stringWidth(ofToString(getCurrentCountdown(), 0));
                    float fx = x + w / 2. - fw / 2.;
                    float fy = h / 2.0 - fontSmall.getSize();// + font.getSize();
                    ofSetColor(255);
                    fontSmall.drawString("Get Ready!", x + w / 2. - fontSmall.stringWidth("Get Ready!") / 2., fy);
                    fy += font.getSize() * 1.5;
                    font.drawString(ofToString(getCurrentCountdown(), 0), fx, fy);
                }
                else {
                    float fy = h / 2.0 - fontSmall.getSize() + 20;// + font.getSize();
                    ofSetColor(255);
                    fontSmall.drawString("Get Ready!", x + w / 2. - fontSmall.stringWidth("Get Ready!") / 2., fy);
                    
                }
            }
                break;
                
            case MODE_PERFORM:
            {
                float fw = font.stringWidth( ofToString(getCurrentCountdown(),0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - fontSmall.getSize() + 20;// + font.getSize();
                ofSetColor(255);
                fontSmall.drawString( "Perform!", x + w/2. - fontSmall.stringWidth( "Perform!" )/2., fy);
                fy += font.getSize() * 1.5;
                font.drawString( ofToString(getCurrentCountdown(),0), fx, fy);
            }
                break;
            case MODE_WATCH:
                float fw = font.stringWidth( ofToString(10,0) );
                float fx = x + w/2. - fw/2.;
                float fy = h/2.0 - fontSmall.getSize() + 20;// + font.getSize();
                ofSetColor(255);
                string s = bVert ? "Share your \nperformance!" : "Share your performance!";
                fontSmall.drawString( s, x + w/2. - fontSmall.stringWidth( s )/2., fy);
                break;
        }
    }
    
    void drawHoriz( float x, float y, float w, float h ){
		ofPushMatrix();
		float s = 1920. / 2560.;
		ofScale(s, s);

        switch (currentMode ){
            case MODE_SELECT:
            {
                x = 460;
                y = 1101;
                // first, draw overlay
                oSelect.draw(0,0);
                
                // hard-corded, what of it?
                
                float cw = 320;
                float ch = 240;
                float sp = 50;
                
                x += 50;
                y += 50;
                
                for ( auto & c : clips ){
                    c.v().setVolume(0);
                    c.v().update();
                    c.draw(x,y,cw,ch);
                    c.set(x * s,y * s,cw * s,ch * s);
                    x += cw + sp;
                }
            }
                break;
            case MODE_PREVIEW:
            {
                float scale = getCurrentCountdownPct();
                
                oPractice.draw(0,0);
                ofSetColor(255);
                ofDrawRectangle(466, 1375, 1626. * scale, 50);
            }
                break;
                
            case MODE_GETREADY:
            {
                float scale = getCurrentCountdownPct();
                
                oPractice.draw(0,0);
                ofSetColor(255);
                ofDrawRectangle(466, 1375, 1626. * scale, 50);
            }
                break;
                
            case MODE_PERFORM:
            {
                float scale = getCurrentCountdownPct();
                oPerform.draw(0,0);
                ofSetColor(255);
                ofDrawRectangle(466, 1375, 1626. * scale, 50);
            }
                break;
            case MODE_WATCH:
            {
                
            }
                break;
                
            case MODE_SHARE:
            {
                oShare.draw(0,0);
            }
                break;
        }
		ofPopMatrix();
    }
};