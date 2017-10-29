#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    Serial.setup("/dev/cu.usbmodem1421",115200);
}

//--------------------------------------------------------------
void ofApp::update(){
    // arduinoでwriteされるbyteをセットする
    setSerialBytes();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // テスト表示 削除予定
    // arduinoのSerial.writeと個数を合わさないとおかしくなるよ！
    for (int index = STEP_FIRST_NUM; index <= SENDED_NUM; index++) {
        printf("%s \n", std::to_string(bytes[index]).c_str());
        string stepStr = ofToString(index) + " : " + ofToString(bytes[index]);
        ofDrawBitmapString(stepStr, 50, 30 * index + 10);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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


void ofApp::setSerialBytes(){
    // serial通信、byteの取得
    if (Serial.available() == SENDED_NUM) {
        for (int index = 0; index < SENDED_NUM; index++) {
            bytes[index] = Serial.readByte();
        }
    }
    
    // serialの初期化
    Serial.flush();
}