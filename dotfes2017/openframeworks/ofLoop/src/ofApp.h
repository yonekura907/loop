#pragma once

#include "ofMain.h"

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
    
        ofSerial Serial;
        int bytes[SENDED_NUM];
    
        /* arduinoでwriteされるbyteをセット */
        void setSerialBytes();
};
