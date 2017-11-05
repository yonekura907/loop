#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "127.0.0.1"
#define RECIEVEPORT 7100
#define SENDPORT 7400

class ofApp : public ofBaseApp{

    // useble variable
    // arduino
    //   wood ball 1-16 (by step)
    //   wood ball 0-8 (by color)
    //   knob 1-6 (by order)(!!now only 3)
    //   knob 0-8 (by value)
    // knobs type
    //   bpm
    //   oscillator
    //   delay
    //   kick
    //   rim
    //   octave
    // from max
    //   bpm
    //   fft
    
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
		
        /* Serial */
        static const int STEP_FIRST_NUM = 1;
        static const int STEP_NUM = 16;
        static const int STEP_SENDED_NUM = 21;
        ofSerial KnocsSerial;
        int steps[STEP_SENDED_NUM];
        static const int KNOB_NUM = 3;
        ofSerial StepsSerial;
        int knobs[KNOB_NUM];

        /* OSC */
        ofxOscReceiver OscReceiver;
        ofxOscSender OscSender;
        float getFFT = 0;
        void setStepsSerial();
        void setKnobsSerial();
        //        void drawVisual();
        void receiveOscBpmAndSendSerialLed();
        void sendOscSteps();
        void sendOscKonbs();

        /* view visual */
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
