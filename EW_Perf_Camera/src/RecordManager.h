//
//  RecordManager.h
//  EW_Perf_Camera
//
//  Created by Brett Renfer on 2/1/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVideoRecorder.h"

class RecordManager {
public:
    
    void setup(){
        // todo: settings
        
        // setup recorder
        vidRecorder.setVideoCodec("mpeg4");
        vidRecorder.setVideoBitrate("800k");
        vidRecorder.setAudioCodec("mp3");
        vidRecorder.setAudioBitrate("192k");
        vidRecorder.setPixelFormat("rgba");
        
        fileName = "mmi_performance_";
        fileExt = ".mp4";
        
        params.setName("Recording params");
        params.add(recordInterval.set("Camera switch interval", 5000, 0, 15000));
        params.add(recordLength.set("Recording length", 15000, 1, 60000));
        params.add(camWidth.set("Video width", 640, 1, 1920) );
        params.add(camHeight.set("Video height", 480, 1, 1080) );
        
        bRecording = false;
        currentBgClip = "";
    }
    
    void update( const ofPixels & cameraOne, const ofPixels & cameraTwo ){
        if(bRecording){
            auto t = ofGetElapsedTimeMillis();
            
            if (( t - lastTime ) >= recordInterval){
                whichCamera = !whichCamera;
                lastTime = t;
            }
            
            if ( t - startTime >= recordLength.get() ){
                ofLogError()<<"STOPPING "<< t <<":"<<(t-startTime)<<":"<<recordLength<<endl;
                stopRecording();
                return;
            }
            
            bool success = false;
            
            if ( whichCamera == 0){
                success = vidRecorder.addFrame(cameraOne);
            } else {
                success = vidRecorder.addFrame(cameraTwo);
            }
            
            if (!success) {
                ofLogWarning("This frame was not added!");
//                stopRecording();
            }
        }
    }
    
    void startRecording( string backgroundClip = "" ){
        cout << "BACKGROUNNNND "<<backgroundClip<<endl;
        
        if ( bRecording ){
            ofLogWarning()<<"Already recording, try again in "<<((ofGetElapsedTimeMillis()-startTime)/1000.)<<" seconds";
            return;
        }
        startTime = lastTime = ofGetElapsedTimeMillis();
        bRecording = true;
        whichCamera = 0;
        
        currentFileName = (fileName.get() +ofGetTimestampString()+fileExt.get() );
        
        currentBgClip = backgroundClip;
        
        vidRecorder.setup(currentFileName, camWidth, camHeight, 30);
        vidRecorder.start();
    }
    
    void stopRecording(){
        bRecording = false;
        vidRecorder.close();
        
        // after that, copy in the audio
        if ( currentBgClip != "" ){
            
            string lastCmd = "bash --login -c 'ffmpeg -i " + currentFileName;
            lastCmd +=" -i "+ currentBgClip + fileExt.get() +" -c copy -map 0:v:0 -map 1:a:0 -shortest "+currentFileName +"_final"+fileExt.get()+"'";
            
            system(lastCmd.c_str() );
            currentFileName = currentFileName + "_final"+fileExt.get();
        } else {
        }
        ofNotifyEvent(onFinishedRecording, currentFileName, this);
        currentBgClip = "";
    }
    
    void close(){
        vidRecorder.close();
    }
    
    ofParameterGroup params;
    
    ofEvent<string> onFinishedRecording;
    
protected:
    
    ofParameter<string> fileName;
    ofParameter<string> fileExt;
    ofParameter<uint64_t> recordInterval;
    ofParameter<uint64_t> recordLength;
    ofParameter<int>    camWidth;
    ofParameter<int>    camHeight;
    
    string currentFileName, currentBgClip;
    
    // recording
    ofxVideoRecorder    vidRecorder;
    bool bRecording;
    
    uint64_t startTime, lastTime;
    int whichCamera;
};
