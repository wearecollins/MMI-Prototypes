#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#define KINECT1

#ifdef KINECT1
#include "ofxKinect.h"
#else
#include "ofxMultiKinectV2.h"
#endif
#include "MovieGrid.h"

#include "ofxSpacebrew.h"

#define STRINGIFY(x) #x

static string depthFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r;
              float low1 = 500.0;
              float high1 = 5000.0;
              float low2 = 1.0;
              float high2 = 0.0;
              float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
              if (d == 1.0) {
                  d = 0.0;
              }
              gl_FragColor = vec4(vec3(d), 1.0);
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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Spacebrew::Connection spacebrew;
        void onMessage( Spacebrew::Message & m );
    
#ifdef KINECT1
        ofxKinect kinect;
#else
        ofxMultiKinectV2 kinect;
#endif
        MovieGrid grid;
        ofShader depthShader;
        ofShader irShader;
    
        ofxCv::ContourFinder contours;
        ofFbo contourFBO, contourFBOOut;
    
};
