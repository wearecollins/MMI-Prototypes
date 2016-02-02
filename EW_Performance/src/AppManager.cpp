#include "AppManager.h"

namespace mmi {
    //--------------------------------------------------------------
    AppManager & AppManager::get()
    {
        static bool bConstructed = false;
        static AppManager inst;
        
        if (!bConstructed){
            inst.params.setName("App settings");
            // do stuff
            inst.params.add(inst.COUNTDOWN_PREVIEW.set("COUNTDOWN_PREVIEW", 15.f, 0., 30.));
            inst.params.add(inst.COUNTDOWN_GETREADY.set("COUNTDOWN_GETREADY", 10.f, 0., 30.));
            inst.params.add(inst.COUNTDOWN_PERFORM.set("COUNTDOWN_PERFORM", 15.f, 0., 30.));
            inst.params.add(inst.COUNTDOWN_WATCH.set("COUNTDOWN_WATCH", 15.f, 0., 30.));
            inst.params.add(inst.GET_READY_ALPHA.set("COUNTDOWN_PREVIEW", 100.f, 0., 255.));
            inst.params.add(inst.doPreview.set("Do clip preview", false));
            inst.params.add(inst.doPractice.set("Do practice", true));
            
            inst.time = ofGetElapsedTimef();
            
            inst.setMode( MODE_INTRO );
            
            bConstructed = true;
        }
        return inst;
    }
    
    //--------------------------------------------------------------
    void AppManager::update(){
        time = ofGetElapsedTimef();
        
        switch (currentMode ){
            case MODE_INTRO:
            case MODE_SELECT:
                break;
                
            case MODE_PREVIEW_1:
                if ( ofGetElapsedTimef() - time >= COUNTDOWN_PREVIEW ){
                    setMode(doPractice ? MODE_GETREADY : MODE_PERFORM);
                }
                break;
                
            case MODE_PREVIEW_2:
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

    //--------------------------------------------------------------
    float AppManager::getCurrentCountdown(){
        float ret = 0.;
        switch (currentMode ){
            case MODE_PERFORM:
                ret =  fmax(0,(COUNTDOWN_PERFORM - (ofGetElapsedTimef() - time)));
                break;
                
            case MODE_GETREADY:
                ret =  fmax(0,(COUNTDOWN_GETREADY - (ofGetElapsedTimef() - time)));
                break;
        }
        return ret;
    }
    
    //--------------------------------------------------------------
    float AppManager::getCurrentCountdownPct(){
        float ret = 0.;
        switch (currentMode ){
            case MODE_PERFORM:
                ret = fmax(0,(ofGetElapsedTimef() - time) / (COUNTDOWN_PERFORM));
                break;
                
            case MODE_GETREADY:
                ret = fmax(0,(ofGetElapsedTimef() - time) / (COUNTDOWN_GETREADY));
                break;
        }
        return ret;
    }
    
    //--------------------------------------------------------------
    Mode AppManager::getMode(){
        return currentMode;
    }

    //--------------------------------------------------------------
    void AppManager::setMode( Mode newMode ){
        if ( currentMode == newMode ) return; // same 'ol
        
        // update time?
        
        time = ofGetElapsedTimef();
        
        switch( newMode ){
            case MODE_INTRO:
                break;
            case MODE_PREVIEW_1:
                break;
            case MODE_PREVIEW_2:
                break;
            case MODE_SELECT:
                break;
            case MODE_GETREADY:
                break;
            case MODE_PERFORM:
                break;
            case MODE_WATCH:
                break;
            case MODE_SHARE:
                break;
        }
        
        currentMode = newMode;
        ofNotifyEvent(onNewMode, this);
    }

    // protected
};