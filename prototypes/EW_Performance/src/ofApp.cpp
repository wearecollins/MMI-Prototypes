#include "ofApp.h"

bool bGuiVisible = false;
bool bAddFront = false;

float cam_scale_factor = .5;
float cam_width = 640.;
float cam_height = 480;

float map_scale_factor = 2.;

#define DEPTH_WIDTH			512
#define DEPTH_HEIGHT		424
#define Z_AXIS_MAGNITUDE	.1

// quick record
uint64_t lastAdded = 0;
uint64_t lastSwitched = 0;
uint64_t rate = 1000/60;
uint64_t shift = 2000;

Mode lastMode = MODE_SELECT;



//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
	cameraLeft.setDeviceID(1);
	cameraLeft.setup(cam_width, cam_height);
	cameraRight.setDeviceID(2);
	cameraRight.setup(cam_width, cam_height);
    
#ifdef TARGET_OSX
    clips.setup();
#else
	clips.setup("videos_win");
#endif
    
    ofBackground(0);
    
    leftScreen.allocate(cam_width, cam_height);//, GL_RGBA, 8);
    rightScreen.allocate(800,1280);
    saver.allocate(512, 424);
    
    recorder.setup();
    ofAddListener(recorder.donePlaying, this, &ofApp::donePlayback);
    
    background.load("background.png");
    
    // create GUI
	gui.setup();
    
	gui.add(useOverlay.set("Draw overlay", false));
	gui.add(rScreenPos.set("right screen pos", ofVec2f(1920, 0), ofVec2f(0, 0), ofVec2f(3840, 1080)));
	gui.add(lScreenPos.set("left screen pos", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(3840, 1080)));
	gui.add(screenRight.set("Screen right/bottom", true));
	gui.add(clips.COUNTDOWN_PREVIEW);
	gui.add(clips.COUNTDOWN_GETREADY);
	gui.add(clips.COUNTDOWN_PERFORM);
	gui.add(clips.COUNTDOWN_WATCH);
	gui.add(clips.GET_READY_ALPHA);
	gui.add(clips.doPractice);
	gui.add(clips.doPreview);
	gui.add(min.set("Kinect mindepth", 0, 0., 5000));
	gui.add(max.set("Kinect max depth", 100, 0., 5000));
	gui.add(doRecord.set("Do recording", true));
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
	cameraLeft.update();
	cameraRight.update();

	static bool bRight = true;
	if (bRight != screenRight.get()) {
		bRight = screenRight.get();

		if (bRight) {
			rightScreen.allocate(800, 1280);
		}
		else {
			rightScreen.allocate(1920, 1200);
		}
	}

	if(clips.getMode() == MODE_PERFORM && doRecord && (cameraLeft.isFrameNew() || cameraRight.isFrameNew()) ) {
		uint64_t t = ofGetElapsedTimeMillis();
		if (t - lastAdded > rate) {
			lastAdded = t;

				static ofPixels pix;
				if (bAddFront) {
					auto & p = cameraLeft.getPixels();
					pix.setFromPixels(p.getData(), p.getWidth(), p.getHeight(), p.getNumChannels());
					pix.setImageType(OF_IMAGE_GRAYSCALE);
					pix.mirror(false, true);

					recorder.addFrame(pix);
				}
				else {
					auto & p = cameraRight.getPixels();
					pix.setFromPixels(p.getData(), p.getWidth(), p.getHeight(), p.getNumChannels());
					pix.setImageType(OF_IMAGE_GRAYSCALE);
					recorder.addFrame( pix );
				}
				if (t - lastSwitched > shift) {
					lastSwitched = t;
					bAddFront = !bAddFront;
				}
		}
	}
    
    if ( clips.getMode() == MODE_SELECT && lastMode != MODE_SELECT ){
        recorder.clear();
    }
    lastMode = clips.getMode();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float bgs = useOverlay ? (float) ofGetWidth() / background.getWidth() : 1.0;
    
    // left screen
    leftScreen.begin();
    {
        ofClear(0);
        
        ofPushStyle();
        ofSetColor(50);
        ofDrawRectangle(0, 0, leftScreen.getWidth(), leftScreen.getHeight());
        ofPopStyle();
        
        // tbd where this goes
        
        //float cam_scale = k_width / 640.;
        //leftCamera.draw(50, 50 + k_height, k_width, leftCamera.getHeight() * cam_scale);

		if (cameraLeft.getTexture().isAllocated()){
            ofPushStyle();
            if ( clips.getMode() == MODE_GETREADY ){
//                ofSetColor(255,150);
            }
            ofSetColor(255);
            ofPushMatrix();
				static ofImage kinectImage;
				kinectImage.setFromPixels(cameraLeft.getPixels());
				kinectImage.setImageType(OF_IMAGE_GRAYSCALE);
				//kinectImage.mirror(false, true);
				kinectImage.update();
				kinectImage.draw(0, 0, cam_width, cam_height);

				ofPopMatrix();
            
            ofPopMatrix();
            ofPopStyle();
        }
        
        if ( clips.getMode() == MODE_PREVIEW || clips.getMode() == MODE_GETREADY ){
            ofPushStyle();
            ofSetColor(0, clips.getMode() == MODE_GETREADY ? ofMap( clips.getCurrentCountdown(), 3, 0, clips.GET_READY_ALPHA, 0, true ) : 255);//,ofMap( clips.getCurrentCountdown(), 3, 0, 150, 0, true ));
            ofDrawRectangle(0, 0, leftScreen.getWidth(), leftScreen.getHeight());
            ofPopStyle();
            clips.drawPreview(screenRight.get(), 0,0,leftScreen.getWidth(), leftScreen.getHeight());
        } else if ( clips.getMode() == MODE_WATCH ){
            recorder.beginRender();
            
            float k_width = (leftScreen.getWidth());
			float scale = k_width / cam_width;
            ofPushMatrix();
            ofTranslate(leftScreen.getWidth()/2.0, leftScreen.getHeight()/2.0);
            ofScale(-1., 1.);
            ofTranslate(-leftScreen.getWidth()/2.0, -leftScreen.getHeight()/2.0);
            ofScale(scale, scale);
            recorder.draw();
            ofPopMatrix();
        }
    }
    leftScreen.end();
    
    rightScreen.begin();
    {
        ofClear(0);
        
        if ( screenRight.get() ){
            ofPushStyle();
            switch( clips.getMode() ){
                case MODE_SELECT:
                    ofSetColor(50);
                    break;
                    
                case MODE_PREVIEW:
                    ofSetHexColor(0xe2d000);
                    break;
                    
                case MODE_GETREADY:
                    ofSetHexColor( 0xe27000);
                    break;
                    
                case MODE_WATCH:
                    ofSetHexColor(0x7500e2);
                    break;
                    
                case MODE_PERFORM:
                    ofSetHexColor(0x6ae200);
                    break;
            }
            ofDrawRectangle(0,0,rightScreen.getWidth(), rightScreen.getHeight());
            ofPopStyle();
        }
        
        clips.draw(screenRight.get(), 0, 0, rightScreen.getWidth(), rightScreen.getHeight());
        
        if ( clips.getMode() == MODE_SHARE ){
            
            float k_width = 640.;
            float scale = k_width / cam_width;
            
            ofPushMatrix();
            ofTranslate(rightScreen.getWidth()/2.0, rightScreen.getHeight()/2.0);
            ofScale(-1., 1.);
            ofTranslate(-rightScreen.getWidth()/2.0, -rightScreen.getHeight()/2.0);
			ofTranslate(rightScreen.getWidth() / 2.0, rightScreen.getHeight() / 2.0);
            ofScale(scale, scale);
			ofTranslate(-640. / 2., -480. / 2.);
            recorder.draw();
            ofPopMatrix();
        }
    }
    rightScreen.end();
    
    float as = .51 * (1./cam_scale_factor);
    float lx = 253 * bgs;
    float ly = 375 * bgs;
    float lw = leftScreen.getWidth() * (ofGetHeight() / leftScreen.getHeight());
    float lh = leftScreen.getHeight() * (ofGetHeight() / leftScreen.getHeight());
    
    leftScreen.draw(lScreenPos.get().x, lScreenPos.get().y, lw, lh );
   
    clips.setPos(rScreenPos.get().x, rScreenPos.get().y, 1.0 );
    rightScreen.draw(rScreenPos.get().x, rScreenPos.get().y);
    
//    ofSetColor(255,100);
	if (useOverlay)	
		background.draw(0, 0, background.getWidth() * bgs, background.getHeight() * bgs );
    
    ofSetColor(255);
    
    if ( bGuiVisible ){
		cameraRight.draw(640, 0);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::donePlayback(){
    clips.setMode(MODE_SHARE);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        //process_occlusion =  !process_occlusion;
    } else if (key == 'f') {
        ofToggleFullscreen();
    } else if ( key == 'g' ){
        bGuiVisible = !bGuiVisible;
    } else if ( key == 's' ){
		gui.saveToFile ("settings.xml");
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
