#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"
#include "ofxGui.h"

#define HOST "127.0.0.1"
#define RECIEVEPORT 7100
#define SENDPORT 7400

class ofApp : public ofBaseApp{

    // useble variable
    // arduino
    //   wood ball 1-16 (by step)
    //   wood ball 1-8 (by color)
    //   knob 1-6 (by order)(!!now only 3)
    //   knob 1-6 (by value)
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
    
        /* debug mode */
        static const bool IS_USE_GUI = true;//not arduino
        ofColor BACKGROUNDCOLLOR = (0);
    
        /* Serial */
        static const int STEP_FIRST_NUM = 1;
        static const int STEP_NUM = 16;
        static const int STEP_SENDED_NUM = 21;
        ofSerial KnocsSerial;
        int steps[STEP_SENDED_NUM];
        static const int KNOB_NUM = 6;
        ofSerial StepsSerial;
        int knobs[KNOB_NUM];
        int getBpm();
        int getOscillator();
        int getDelay();
        int getKick();
        int getRim();
        int getOctave();
        int roundCount = 0;

        /* OSC */
        int nowstep = 0;
        ofxOscReceiver OscReceiver;
        ofxOscSender OscSender;
        float getFFT = 0;
        void setStepsSerial();
        void setKnobsSerial();
        void updateStepsSerialDebug();
        void updateKnobsSerialDebug();
        void receiveOscBpmAndSendSerialLed();
        void sendOscSteps();
        void sendOscKonbs();

        /* view visual */
        void drawVisualizer();
        float drawStartTimer = 0;
        /* SinCos */
        void drawSinCos();
        float drawSinCos_rot;
        /* CircleWave */
        void drawCircleWave();
        float getBpmSpeed();
        static const int drawCircleWave_CIRCLE_COUNT = 100;
        float drawCircleWave_yvalues[drawCircleWave_CIRCLE_COUNT];
        float drawCircleWave_theta = 0;
        float drawCircleWave_r = 15;
        float drawCircleWave_h = 100;
        /* ParticleSphere */
        void setupParticleSphere();
        void drawParticleSphere();
        float drawParticleSphere_circleSize = 400;
        float drawParticleSphere_radius = drawParticleSphere_circleSize/2;
        int drawParticleSphere_period = 5; //speed
        float drawParticleSphere_x = 0;
        float drawParticleSphere_particleY;
        double drawParticleSphere_amplitude;
        float drawParticleSphere_sineEl;
        float drawParticleSphere_r = 0;
        int drawParticleSphere_randomPart[100];
        int drawParticleSphere_partSize[100];
        /* CircleTree */
        void drawCircleTree();
        float drawCircleTree_A = 120;
        float drawCircleTree_frequency = .0314;
        float drawCircleTree_time = 0;
        float drawCircleTree_diameter = 10;
        float drawCircleTree_radius = drawCircleTree_diameter/2;
        float drawCircleTree_col;
        float drawCircleTree_coll;
        float drawCircleTree_r;
        /* view visual common function */
        int getSameStepNum(int value);
        /* debug */
        void drawKnobValue();
    
        /* box2d */
        ofxBox2d box2d;
        vector <ofPtr<ofxBox2dCircle>> box2dCircles;
        vector <ofPtr<ofxBox2dRect>> box2dRects;
        vector <ofColor> box2dCirclesColor;
        void addBox2dCircles(int step, int stepValue);
        void drawBox2dCircles();
    
        /* processing method */
        int windowWidth;
        int windowHeight;
        void background(int rgb);
        void background(int r, int g, int b);
        void translate(float x, float y);
        void rotate(float degrees);
        float radians(float degree);
        float millis();
        float startTimer;
        float map(float v, float sx, float sn, float dx, float dn);
        void fill(ofColor color);
        void fill(float r, float g, float b);
        void strokeWeight(float lineWidth);
        void ellipse(float x, float y, float width, float height);
        float sq(float value);
    
        /* meke polygon method */
        void triangle(float x, float y, float width, float height);
        void rectangle(float x, float y, float width, float height);
        void pentagon(float x, float y, float width, float height);
        void hexagon(float x, float y, float width, float height);
        void heptagon(float x, float y, float width, float height);
        void octagon(float x, float y, float width, float height);
    
        /* oscillator form */
        void drawOscillatorForm(float x, float y, float width, float height);
    
        /* octobe reverce form */
        int getReverceOctave();
    
        /* wooden ball color */
        ofColor getWoodenBallColor(int index);

        /* delay reaction */
        // void setDelayReaction();
    
        /* GUI */
        ofxPanel gui;
        ofxIntSlider stepsSlider[STEP_SENDED_NUM];
        ofxIntSlider knobsSlider[KNOB_NUM];
};
