//
//  AppManager.h
//  EW_Performance
//
//  Created by Brett Renfer on 7/8/15.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

namespace mmi {
    
    class AppManager {
    public:
        
        static AppManager & get();
        
        // public gui-able vars
        ofParameter<float> COUNTDOWN_PREVIEW;
        ofParameter<float> COUNTDOWN_GETREADY;
        ofParameter<float> COUNTDOWN_PERFORM;
        ofParameter<float> COUNTDOWN_WATCH;
        ofParameter<bool>  doPreview, doPractice;
        ofParameter<float> GET_READY_ALPHA;
        
        ofParameterGroup params;
        
        // tick
        void update();
        
        // getters
        Mode getMode();
        
        float getCurrentCountdown();
        float getCurrentCountdownPct();
        
        // setters
        void setMode( Mode newMode );
        
        ofEvent <void> onNewMode;
        
    protected:
        
        AppManager(){
        }
        
        Mode    currentMode;
        float   time;
    };
    
    // settings
    class Settings {
    public:
        
        static Settings& get()
        {
            static Settings inst; // Guaranteed to be destroyed.
            static bool bInstance = false;
            
            if ( !bInstance ){
                if ( inst.settingsFile == "" ){
                    inst.settingsFile = "settings.xml";
                }
                
                inst.load(inst.settingsFile);
                
                bInstance = true;
            }
            
            return inst;
        }
        
        static void save()
        {
            auto & inst = get();
            ofXml settings;
            settings.addChild("settings");
            settings.setTo("settings");
            // add values
            settings.setToParent();
            settings.save(ofToDataPath( inst.settingsFile ));
        }
        
        void load( string file ){
            settingsFile = file;
            
            // these should be inside 'Resources'
            ofXml settings;
            if (settings.load(ofToDataPath( settingsFile ))){
                // set stuff
            }
        }
        
    private:
        
        Settings(){
            settingsFile = "";
        };
        
        string settingsFile;
        
        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
        Settings(Settings const&)               = delete;
        void operator=(Settings const&)  = delete;
    };
    
}


