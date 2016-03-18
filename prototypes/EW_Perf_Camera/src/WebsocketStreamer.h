//
//  WebsocketStreamer.h
//  EW_Perf_Camera
//
//  Created by Brett Renfer on 2/5/16.
//
//

#pragma once

#include "ofxLibwebsockets.h"
#include "ofxTurboJpeg.h"

class WebsocketStreamer {
    friend class ofxLibwebsockets::Server;
public:
    
    void setup( int port = 9000 ){
        bConnected = bSetup = false;
        ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
        options.port = port;
        
        bSetup = server.setup( options );
        server.addListener(this);
        
        jpegquality = 100;
    }
    
    void stream( ofPixels & pixels ){
        if ( bConnected ){
            static unsigned long size;
            
            // compress video into image via turbojpg
            // the second param == quality. play with this to get a better framerate
            // you can also try resizing your image!
            //currentImage.resize(160, 120);
            unsigned char * compressed = turbo.compressPixels(pixels, jpegquality, &size);
            server.sendBinary(compressed, size);
            free(compressed);
        }
    }
    
    void streamFbo( ofFbo & tex ){
        if ( bConnected ){
            static unsigned long size;
            static ofPixels newPixels;
            tex.readToPixels(newPixels);
            
            unsigned char * compressed = turbo.compressPixels(newPixels, jpegquality, &size);
            server.sendBinary(compressed, size);
            free(compressed);
        }
    }
    
    bool isConnected(){
        return bConnected;
    }
    
    void setQuality( int q ){
        jpegquality = q;
    }
    
protected:
    
    bool bSetup, bConnected;
    int jpegquality;
    ofxTurboJpeg turbo;
    
    ofxLibwebsockets::Server server;
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args ){
        bConnected= true;
    };
    
    void onOpen( ofxLibwebsockets::Event& args ){
        bConnected = true;
    };
    
    void onClose( ofxLibwebsockets::Event& args ){};
    void onIdle( ofxLibwebsockets::Event& args ){};
    void onMessage( ofxLibwebsockets::Event& args ){};
    void onBroadcast( ofxLibwebsockets::Event& args ){};
};
