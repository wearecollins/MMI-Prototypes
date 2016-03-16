#include "ClipScreen.h"

namespace mmi {
    
#pragma mark clip
    
    //--------------------------------------------------------------
    bool Clip::setup( string path, string name, string perf ){
        movie.load(path);
        movie.setVolume(0);
        width = movie.getWidth();
        height = movie.getHeight();
        bOver = false;
        
        this->title= name;
        this->performers = perf;
        
        return true;
    }
        
        
    //--------------------------------------------------------------
    void Clip::draw(){
        movie.update();
        ofPushMatrix();
        ofTranslate(this->x, this->y);
        movie.draw(0,0, this->width, this->height);
        
        auto & fontA = ofxAssets::font(FONT_HEAD, FONT_BUTTON_SIZE);
        auto & fontB = ofxAssets::font(FONT_LIGHT, FONT_BUTTON_SIZE);
        fontA.drawString(this->title, this->x, this->y + this->height + FONT_BUTTON_SIZE );
        fontB.drawString(this->performers, this->x, this->y + this->height + FONT_BUTTON_SIZE );
        
        ofPopMatrix();
    }
    
    //--------------------------------------------------------------
    void Clip::draw( float x, float y, float w, float h ){
        movie.update();
        if ( w == 0 ) w = this->width;
        if ( h == 0 ) h = this->height;
        movie.draw(x,y,w,h);
        if ( bOver ){
            ofSetColor(255,100);
            ofDrawRectangle(x,y,w,h);
            ofSetColor(255);
        }
        
        // set my values to match latest/greatest
        this->x = x;
        this->y = y;
        this->width = w;
        this->height = h;
    }
    
    //--------------------------------------------------------------
    ofVideoPlayer & Clip::v(){
        return movie;
    }
    
#pragma mark ClipScreen
    
    //--------------------------------------------------------------
    void ClipScreen::setup( Mode myMode ){
        Screen::setup(myMode);
        currentClip = nullptr;
        
        ofXml settings;
        bool bSettingsLoaded = settings.load("videos.xml");
        if ( bSettingsLoaded ){
            
            settings.setTo("settings");
            int n = settings.getNumChildren();
            
            for ( int i=0; i<n; i++ ){
                settings.setToChild(i);
                clips.push_back( Clip());
                clips.back().setup( settings.getValue("file"),
                                    settings.getValue("name"),
                                    settings.getValue("performers")
                                   );
                
                clips.back().v().play();
                clips.back().v().setVolume(0);
                //            clips.back().v().setFrame(0);
                clips.back().v().setPaused(true);
                settings.setToParent();
            }
            
            settings.setToParent();
        }
        
        ofAddListener(ofEvents().mousePressed, this, &ClipScreen::onMousePressed);
    }
    
    // to-do: destructor
    
    //--------------------------------------------------------------
    void ClipScreen::update(){
        Screen::update();
    }
    
    //--------------------------------------------------------------
    void ClipScreen::draw(){
        Screen::draw();
        
        float x = 0;
        float y = 0;
        float sp = 10;
        
        static float total_w = -1;
        static float total_h = -1;
        
        if ( total_w == -1){
            
            for ( auto & c : clips ){
                total_w += c.v().getWidth();
                
                if ( c.v().getHeight() > total_h)
                    total_h = c.v().getHeight();
            }
            
            total_w += sp * (clips.size() - 1);
        }
        
        float scale = ofGetWidth()/total_w;
        
        y = ofGetHeight()/2.0 - (total_h * scale)/2.;
        
        for ( auto & c : clips ){
            float c_w = c.v().getWidth() * scale;
            float c_h = c.v().getHeight() * scale;
            
            if ( &c == currentClip){
                ofSetColor(150);
                ofDrawRectangle(x, y, c_w, c_h);
                ofSetColor(255);
            } else
                c.draw(x,y,c_w, c_h);
            
            x += c_w;
            x += sp;
        }
        
        // draw 'preview' if selected clip
        if ( currentClip != nullptr ){
            drawPreview();
        }
    }
    
    //--------------------------------------------------------------
    void ClipScreen::drawPreview(){
        // overlay
        ofPushStyle();
        ofSetColor(0,150);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        
        ofSetColor(255);
        
        if ( currentClip != nullptr ){
            auto & v = currentClip->v();
            float w = (ofGetWidth() * 2/3);
            float s = w/currentClip->v().getWidth();
            float x = ofGetWidth()/2.0 - ( v.getWidth() * s/2.0 );
            float y = ofGetHeight()/2.0 - ( v.getHeight() * s/2.0 );
            currentClip->draw(x, y, v.getWidth() * s, v.getHeight() * s);
        }
        ofPopStyle();
    }
    
    
    //--------------------------------------------------------------
    void ClipScreen::setPos( float x, float y, float s){
        mPos.x = x;
        mPos.y = y;
        mPos.z = s;
    }
    
    void ClipScreen::onStart(){
        Screen::onStart();
        
        for (auto & c : clips ){
            c.v().play();
        }
    }
    
    void ClipScreen::onEnd(){
        Screen::onEnd();
    }
    
    //--------------------------------------------------------------
    void ClipScreen::onMousePressed( ofMouseEventArgs & e ){
        
        ofVec2f m(e.x,e.y);
        
        for ( auto & c : clips ){
            if ( c.inside(m)){
                if ( currentClip != nullptr ){
                    currentClip->v().setVolume(0.);
                    currentClip->v().setPosition(0);
                    currentClip = nullptr;
                } else {
                    currentClip = &c;
                    currentClip->v().setPosition(0);
                    currentClip->v().play();
                    currentClip->v().setVolume(1.);
                }
                break;
            }
        }
    }
    
    //--------------------------------------------------------------
    void ClipScreen::onMouseMoved( ofMouseEventArgs & e ){
    }
    
}