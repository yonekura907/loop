#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    Serial.setup("/dev/cu.usbmodem1421",115200);
    
    OscReceiver.setup(PORT);
    OscSender.setup(HOST, PORT);
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
    
    drawOSC();
    drawSample();
    
//    ofxOscMessage m;
//    m.addIntArg(1);
//    OscSender.sendMessage(m);
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

int ofApp::getPutStepCount(){
    int stepCount = 0;
    for (int index = STEP_FIRST_NUM; index <= SENDED_NUM; index++) {
        if (bytes[index] > 0) stepCount++;
    }
    return stepCount;
}

void ofApp::drawOSC(){
    
    string msg = "";
    msg += "FPS: " + ofToString( ofGetFrameRate() ) + "\n";
    msg += "receiver: " + ofToString( OscReceiver.hasWaitingMessages() ) + "\n";
    
    int index = 0;
    while(OscReceiver.hasWaitingMessages()) {
        
        ofxOscMessage m;
        OscReceiver.getNextMessage(&m);
        string value = m.getArgAsString(index);
//        msg += "getAddress: " + m.getAddress() + "\n";
//        msg += "getArgAsString " + ofToString(index) + ": " + value + "\n";
        
        // arduino MEGAが場合の解決策
        // いきなりランプは点灯できない
        if (millis() > 2000) {
            unsigned char myByte =std::stoi(value);
            bool byteWasWritten = Serial.writeByte(myByte);
            if (!byteWasWritten) printf("もう一度arduinoをつなぎ直してください \n");
        }
        
        
        if( index == 0 ){
            getFFT = ofMap( ofToFloat( m.getArgAsString(index) ), -90, 0, 0, 1 );
        }
        
        index++;
    }
    
//    drawVisual();
    
    msg += "FFT: " + ofToString( getFFT ) + "\n";
    ofSetColor( 0, 0, 0, 255 );
    ofDrawBitmapString( msg, 30, 30 );
    
}

void ofApp::drawVisual(){
    
    float ww = ofGetWidth();
    float wh = ofGetHeight();
    
    ofDrawCircle( ww * 0.5, wh * 0.5, 100 * getFFT );
    
}

//--------------------------------------------------------------
void ofApp::drawSample(){
    float circle = 200;
    float rot;
    float col;
    float freq = 0.000005;
    float cont = 0;
    float r;
    
    float shake = getPutStepCount() * 10;
    float sphereRadius = getPutStepCount() * 3;
    
    background(0);
    translate(300,300);
    rotate(radians(rot));
    
    //これ以下のellipseは半径で描きますよ
    //ellipseMode(RADIUS);
    for (float i=0; i<500; i+=5) {
        
        //座標
        circle = 200 + shake*sin(millis()*freq*i);
        
        //色
        col = map(circle, 150, 250, 255, 60);
        
        //半径
        r = map(circle, 150, 250, sphereRadius, 2);
        
        //   赤　　緑　　青
        fill(col, 255, 255);
        //noStroke();
        //stroke(col, 0, 74);
        ellipse(circle*cos(i), circle*sin(i),r,r);
        rot = rot+0.00005;
    }
}

void ofApp::background(int rgb){
    background(rgb, rgb, rgb);
}

void ofApp::background(int r, int g, int b){
    ofBackground(r, g, b);
}

void ofApp::translate(float x, float y){
    ofTranslate(x, y);
}

void ofApp::rotate(float degrees){
    ofRotate(degrees);
}

float ofApp::radians(float degree){
    return degree * (PI / 180);
}

float ofApp::millis(){
    return ofGetElapsedTimeMillis() - startTimer;
}

float ofApp::map(float v, float sx, float sn, float dx, float dn){
    return (v - sn) * (dx - dn) / (sx - sn) + dn;
}

void ofApp::fill(float r, float g, float b){
    ofSetColor(r,g,b);
    ofFill();
}

void ofApp::ellipse(float x, float y, float width, float height){
    ofEllipse(x,y,width,height);
}