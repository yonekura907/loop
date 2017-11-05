#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "127.0.0.1"
#define PORT 7100

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
		
        /* ステップ数 */
        static const int STEP_FIRST_NUM = 1;
        static const int STEP_NUM = 16;
        static const int SENDED_NUM = 21;
    
        int getPutStepCount();
    
        ofSerial Serial;
        int bytes[SENDED_NUM];
    
        /* arduinoでwriteされるbyteをセット */
        void setSerialBytes();
    
        /* OSC */
        void drawVisual();
        void drawOSC();
        ofxOscReceiver OscReceiver;
        ofxOscSender OscSender;
        float getFFT = 0;
    
        /* visual */
        void drawSample();
    
        /* processing method */
        void background(int rgb);
        void background(int r, int g, int b);
        void translate(float x, float y);
        void rotate(float degrees);
        float radians(float degree);
        float millis();
        float startTimer;
        float map(float v, float sx, float sn, float dx, float dn);
        void fill(float r, float g, float b);
        void ellipse(float x, float y, float width, float height);
        int width;
        int height;

};
