#pragma once

#include "ofMain.h"
#include "RecManager.h"
#include "ClipManager.h"

#ifdef TARGET_OSX
#include "ofxMultiKinectV2.h"#define STRINGIFY(x) #x

static string depthFragmentShader =
STRINGIFY(
	uniform sampler2DRect tex;
void main()
{
	vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
	float value = col.r;
	float low1 = 500.0;
	float high1 = 1000.0;
	float low2 = 1.0;
	float high2 = 0.0;
	float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
	if (d == 1.0) {
		d = 0.0;
	}
	gl_FragColor = vec4(vec3(d), 1.0);
}
);

static string occlusionFragmentShader =
STRINGIFY(
	uniform sampler2DRect depthTex;
uniform sampler2DRect tex;
void main()
{
	vec4 col = texture2DRect(depthTex, gl_TexCoord[0].xy * vec2(.5, .5));
	float value = col.r;
	float low1 = 500.0;
	float high1 = 1000.0;
	float low2 = 1.0;
	float high2 = 0.0;
	float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
	if (d == 1.0) {
		d = 0.0;
	}

	vec4 color = texture2DRect(tex, gl_TexCoord[0].xy);

	if (d == 0.0) {
		gl_FragColor = vec4(vec3(d), 1.0);
	}
	else {
		gl_FragColor = color;
	}
}
);

static string irFragmentShader =
STRINGIFY(
	uniform sampler2DRect tex;
void main()
{
	vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
	float value = col.r / 65535.0;
	gl_FragColor = vec4(vec3(value), 1.0);
}
);

#include "GpuRegistration.h"
#else
#include "ofxKinectForWindows2.h"
#include "ofxKinect.h"
#endif
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // bg
        ofImage background;
    
        // ofxUI
        ofxPanel gui;
		ofParameter<bool> useOverlay, doRecord;
		ofParameter<ofVec2f> lScreenPos,rScreenPos;
		ofParameter<float> min, max;
    
        // clipz
        ClipManager clips;
    
        // pupeteering camera
        ofVideoGrabber leftCamera;
    
        // recording
        RecManager recorder;

#ifdef TARGET_OSX
		// kinect stuff
		ofShader depthShader;
		ofShader irShader;
		ofShader colorDepthShader;

        ofxMultiKinectV2 kinect0;
        ofTexture colorTex0;
        ofTexture depthTex0;
        ofTexture irTex0;
        GpuRegistration gr;
		bool process_occlusion;
#else
		ofxKFW2::Device kinect0;
		ofxKinect camera2;
		ofShader shader;
#endif

		ofFbo leftScreen, rightScreen, saver;
    
        ofMesh test;
};
