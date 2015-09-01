#include "ofApp.h"

bool bGuiVisible = false;
bool bAddFront = false;

float cam_scale_factor = .5;
float cam_width = 1920.* cam_scale_factor;
float cam_height = 1080 * cam_scale_factor;

float map_scale_factor = 2.;

#define DEPTH_WIDTH			512
#define DEPTH_HEIGHT		424
#define Z_AXIS_MAGNITUDE	.1

//#define USE_3D

// quick record
uint64_t lastAdded = 0;
uint64_t lastSwitched = 0;
uint64_t rate = 1000/60;
uint64_t shift = 2000;

Mode lastMode = MODE_SELECT;

ofParameter<bool> bUseKinect;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
#ifdef TARGET_OSX
	process_occlusion = false;
    depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
    depthShader.linkProgram();
    
    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
    irShader.linkProgram();
    
    colorDepthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, occlusionFragmentShader);
    colorDepthShader.linkProgram();
    
    kinect0.open(true, true, 0);
    // Note :
    // Default OpenCL device might not be optimal.
    // e.g. Intel HD Graphics will be chosen instead of GeForce.
    // To avoid it, specify OpenCL device index manually like following.
    // kinect1.open(true, true, 0, 2); // GeForce on MacBookPro Retina
    
    kinect0.start();
    
    gr.setup(kinect0.getProtonect(), map_scale_factor);
	gui->addToggle("Process occlusion mask", &process_occlusion);
#else

	kinect0.open();
	//kinect0.initDepthSource();
	kinect0.initColorSource();

	shader.load("", "render.frag");

	camera2.init();
	camera2.open();

#endif
//    leftCamera.setup(640, 480, true);
    
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
    
    background.load("background.png");
    
    // create GUI
	gui.setup();
    
	gui.add(bUseKinect.set("Use Kinect Color", true));
	gui.add(useOverlay.set("Draw overlay", false));
	gui.add(rScreenPos.set("right screen pos", ofVec2f(1920, 0), ofVec2f(0, 0), ofVec2f(3840, 1080)));
	gui.add(lScreenPos.set("left screen pos", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(3840, 1080)));
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
	kinect0.update();
#ifdef TARGET_OSX

	//    leftCamera.update();
	if (kinect0.isFrameNew()) {
		static ofPixels colorPix;
		colorPix.swap(kinect0.getColorPixelsRef());
		colorPix.setImageType(OF_IMAGE_GRAYSCALE);
		colorTex0.loadData(colorPix);
		depthTex0.loadData(kinect0.getDepthPixelsRef());
		//        irTex0.loadData(kinect0.getIrPixelsRef());

		depthTex0.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
		gr.update(depthTex0, colorTex0, process_occlusion);

#ifdef USE_3D
		test.clear();

		for (int y = 0; y < DEPTH_HEIGHT; y++) {
			for (int x = 0; x < DEPTH_WIDTH; x++) {
				int index = x + (y*DEPTH_WIDTH);
				float z = Z_AXIS_MAGNITUDE * kinect0.getDepthPixelsRef().getData()[index];

				// IGNORE THE HIGH AND LOW VALUES
				if ((z == 0) || (z == Z_AXIS_MAGNITUDE)) continue;

				// CREATE A POINT WITH THE SAME X AND Y BY A Z VALUE MATCHING THE DEPTH
				ofVec3f newPoint(x - DEPTH_WIDTH / 2, (DEPTH_HEIGHT / 2) - y, z);
				test.addColor(ofFloatColor(1.));
				test.addVertex(newPoint);
				test.addTexCoord(ofVec2f(ofMap(x, 0, DEPTH_WIDTH, 0., DEPTH_WIDTH * map_scale_factor),// ofMap(mouseX, 0, ofGetWidth(), 0, 1920) ),
					ofMap(y, 0, DEPTH_HEIGHT, 0., DEPTH_HEIGHT * map_scale_factor)// ofMap(mouseY, 0, ofGetHeight(), 0, 1080))
					));
			}
		}

#endif
        
        if ( clips.getMode() == MODE_PERFORM ){
            if ( ofGetElapsedTimeMillis() - lastAdded > rate ){
                lastAdded = ofGetElapsedTimeMillis();
                
                if ( bUseKinect ){
                    saver.begin();
                    ofClear(0);
                    colorDepthShader.begin();
                    colorDepthShader.setUniformTexture("depthTex", depthTex0, 1);
                    colorDepthShader.setUniformTexture("tex", gr.getRegisteredTexture(process_occlusion), 2);
                    gr.getRegisteredTexture(process_occlusion).draw(0, 0, 512, 424);
                    colorDepthShader.end();
                    saver.end();
                    
                    static ofPixels pix;
                    saver.readToPixels(pix);
                    recorder.addFrame(pix);
                } else {
                    recorder.addFrame(kinect0.getColorPixelsRef());
                }
            }
        }
    }
#else
	camera2.update();

	if(clips.getMode() == MODE_PERFORM && doRecord) {
		uint64_t t = ofGetElapsedTimeMillis();
		if (t - lastAdded > rate) {
			lastAdded = t;

			if (bUseKinect) {
				//recorder.addFrame(kinect0.getColorSource()->getPixels());
			}
			else {
				static ofPixels pix;
				if (bAddFront) {
					auto & p = kinect0.getColorSource()->getPixels();
					pix.setFromPixels(p.getData(), p.getWidth(), p.getHeight(), p.getNumChannels());
					pix.resize(cam_width, cam_height);
					pix.setImageType(OF_IMAGE_GRAYSCALE);

					recorder.addFrame(pix);
				}
				else {
					pix.setFromPixels(camera2.getPixels().getData(), camera2.getPixels().getWidth(), camera2.getPixels().getHeight(), OF_IMAGE_COLOR);
					
					float s = MIN(cam_width / pix.getWidth(), cam_height / pix.getHeight());
					float w = pix.getWidth();
					float h = pix.getHeight();

					pix.resize(cam_width, cam_height);
					pix.setImageType(OF_IMAGE_GRAYSCALE);
					recorder.addFrame( pix );
				}
				if (t - lastSwitched > shift) {
					lastSwitched = t;
					bAddFront = !bAddFront;
				}
			}
		}
	}
#endif
    
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
#ifdef TARGET_OSX
        if (colorTex0.isAllocated()) {
#else 
		if (kinect0.getColorSource()->getTexture().isAllocated()){
#endif
            ofPushStyle();
            if ( clips.getMode() == MODE_GETREADY ){
//                ofSetColor(255,150);
            }
            
#ifdef USE_3D
            ofPushMatrix();
            ofTranslate(512,424);
            ofScale(1.5, -1.5,1.5);
            glPointSize(1.);
            gr.getRegisteredTexture(process_occlusion).bind();
            test.drawVertices();
            gr.getRegisteredTexture(process_occlusion).unbind();
            ofPopMatrix();
#else
            ofSetColor(255);
            ofPushMatrix();
            
            if ( bUseKinect ){
				float s = (leftScreen.getWidth()) / 512.;
    //            depthShader.begin();
    //            depthTex0.draw(p, y, k_width, k_height);
    //            depthShader.end();
                
    //            k_height = (1080) * k_width / 1920.;
                
                ofTranslate(leftScreen.getWidth() /2.0, leftScreen.getHeight() /2.0);
                ofScale(-1., 1.);
                ofTranslate(-leftScreen.getWidth() / 2.0, -leftScreen.getHeight() / 2.0);
                
                ofSetColor(255);
#ifdef TARGET_OSX
                colorDepthShader.begin();
                colorDepthShader.setUniformTexture("depthTex", depthTex0, 1);
                colorDepthShader.setUniformTexture("tex", gr.getRegisteredTexture(process_occlusion), 2);
                gr.getRegisteredTexture(process_occlusion).draw(0, 0, k_width, k_height);
                colorDepthShader.end();
#else ofPushMatrix();

				auto & cTex = kinect0.getColorSource()->getTexture();
				auto & dTex = kinect0.getDepthSource()->getTexture();

				//dTex.bind(1);

				auto & pix = kinect0.getDepthSource()->getPixels();
				auto & colorPix = kinect0.getColorSource()->getPixels();
				auto * map = kinect0.getDepthSource()->coordinateMapper;
				ColorSpacePoint * csp = new ColorSpacePoint[512 * 424];

				map->MapDepthFrameToColorSpace(512 * 424, pix, 512 * 424, csp);

				ofPixels colorPixels;
				colorPixels.allocate(512, 424, OF_IMAGE_COLOR);
				colorPixels.setColor(ofColor(0));

				for (int x = 0; x < 512; x++) {
					for (int y = 0; y < 424; y++) {
						int ind = (y * 512) + x;

						auto cp = csp[ind];

						int cX = (int)floor(cp.X + .5);
						int cY = (int)floor(cp.Y + .5);

						bool depthGood = false;
						float d = (float)pix.getData()[ind * pix.getNumChannels()];
						if (d > min && d < max) {
							depthGood = true;
						}

						if (cX >= 0 && cX < 1920 && cY >= 0 && cY < 1080 && depthGood) {
							colorPixels.setColor(x, y, colorPix.getColor(cX, cY));
						}

					}
				}

				ofTexture drawTex;
				drawTex.loadData(colorPixels);
				drawTex.draw(0, 0, drawTex.getWidth() * s, drawTex.getHeight() * s);

				//shader.begin();
				/*shader.setUniformTexture("colorFrame", cTex, 0);
				shader.setUniformTexture("depthFrame", dTex, 1);
				shader.setUniform1f("min", min.get());
				shader.setUniform1f("max", max.get());*/


				//shader.end();
#endif
            } else {
                ofTranslate(cam_width /2.0, cam_height /2.0);
                ofScale(-1., 1.);
                ofTranslate(-cam_width /2.0, -cam_height /2.0);
#ifdef TARGET_OSX
                colorTex0.draw(0, 0, k_width, k_height);
#else
				static ofImage kinectImage;
				kinectImage.setFromPixels(kinect0.getColorSource()->getPixels());
				kinectImage.setImageType(OF_IMAGE_GRAYSCALE);
				kinectImage.update();
				kinectImage.draw(0, 0, cam_width, cam_height);

				ofPopMatrix();
#endif
            }
            
            ofPopMatrix();
#endif
            ofPopStyle();
        }
        
        if ( clips.getMode() == MODE_PREVIEW || clips.getMode() == MODE_GETREADY ){
            ofPushStyle();
            ofSetColor(0, clips.getMode() == MODE_GETREADY ? ofMap( clips.getCurrentCountdown(), 3, 0, clips.GET_READY_ALPHA, 0, true ) : 255);//,ofMap( clips.getCurrentCountdown(), 3, 0, 150, 0, true ));
            ofDrawRectangle(0, 0, leftScreen.getWidth(), leftScreen.getHeight());
            ofPopStyle();
            clips.drawPreview(0,0,leftScreen.getWidth(), leftScreen.getHeight());
        } else if ( clips.getMode() == MODE_WATCH ){
            float k_width = (leftScreen.getWidth());
            float scale = k_width / (bUseKinect ? 512. : cam_width);
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
        
        clips.draw(0, 150, rightScreen.getWidth(), rightScreen.getHeight() - 400.);
        
    }
    rightScreen.end();
    
    float as = .51 * (1./cam_scale_factor);
    float lx = 253 * bgs;
    float ly = 375 * bgs;
    float lw = leftScreen.getWidth() * (ofGetHeight() / leftScreen.getHeight());
    float lh = leftScreen.getHeight() * (ofGetHeight() / leftScreen.getHeight());
    
    leftScreen.draw(lScreenPos.get().x, lScreenPos.get().y, lw, lh );
   
    clips.setPos(rScreenPos.get().x, rScreenPos.get().y, 1.0 );
    rightScreen.draw(rScreenPos.get().x, rScreenPos.get().y );
    
//    ofSetColor(255,100);
	if (useOverlay)	
		background.draw(0, 0, background.getWidth() * bgs, background.getHeight() * bgs );
    
    ofSetColor(255);
    
    if ( bGuiVisible ){
		camera2.draw(640, 0);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        //process_occlusion =  !process_occlusion;
    } else if (key == 'f') {
        ofToggleFullscreen();
    } else if ( key == 'k' ){
        bUseKinect = !bUseKinect;
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
