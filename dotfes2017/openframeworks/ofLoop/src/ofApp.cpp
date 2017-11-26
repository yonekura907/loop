#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(BACKGROUNDCOLLOR);
    
    if (IS_USE_GUI) {
        // GUI
        gui.setup();
        for (int index = 0; index < STEP_SENDED_NUM; index++) {
            // (name, init, min, max)
            gui.add(stepsSlider[index].setup("step" + ofToString(index), 0, 0, 8));
        }
        for (int index = 0; index < KNOB_NUM; index++) {
            gui.add(knobsSlider[index].setup("knob" + ofToString(index), 1, 1, 6));
        }
    } else {
        // arduino (serial)
        StepsSerial.setup("/dev/cu.usbmodem1411",115200);
        KnocsSerial.setup("/dev/cu.usbmodem1421",115200);
    }

    // max (OSC)
    OscReceiver.setup(RECIEVEPORT);
    OscSender.setup(HOST, SENDPORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    // processing method
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    if (IS_USE_GUI) {
        // GUI
        updateStepsSerialDebug();
        updateKnobsSerialDebug();
    } else {
        // serial
        setStepsSerial();
        setKnobsSerial();
    }
    
    // OSC
    sendOscSteps();
    sendOscKonbs();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // recieve OSC.
    receiveOscBpmAndSendSerialLed();
    
    // draw visual.
    drawVisualizer();
    
    if (IS_USE_GUI) {
        // GUI
        gui.draw();
    }
    
    // check knob value from arduino(serial)
    // drawKnobValue();
}

//--------------------------------------------------------------
void ofApp::drawVisualizer(){
    if (roundCount >= 0 && roundCount < 3) {
        drawCircleWave();
    } else if (roundCount >= 3 && roundCount < 6) {
        drawSinCos();
    } else if (roundCount >= 6 && roundCount < 9) {
        drawCircleTree();
    } else if (roundCount >= 9) {
        drawCircleWave();
        roundCount = 0;
    }
}

//--------------------------------------------------------------
void ofApp::drawKnobValue(){
        string msg = "";
        msg += "bpm: " + ofToString( knobs[0] ) + "\n";
        msg += "oscillator: " + ofToString( knobs[1] ) + "\n";
        msg += "delay: " + ofToString( knobs[2] ) + "\n";
        msg += "kick: " + ofToString( knobs[3] ) + "\n";
        msg += "rim: " + ofToString( knobs[4] ) + "\n";
        msg += "octave: " + ofToString( knobs[5] ) + "\n";
        ofDrawBitmapString( msg, 30, 30 );
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

//--------------------------------------------------------------
// get steps value from arduino (MEGA)
//--------------------------------------------------------------
void ofApp::setStepsSerial(){
    if (StepsSerial.available() == STEP_SENDED_NUM) {
        for (int index = 0; index < STEP_SENDED_NUM; index++) {
            steps[index] = StepsSerial.readByte();
        }
    }
    StepsSerial.flush();
}

//--------------------------------------------------------------
// get knobs value from arduino (LEONALDO)
//--------------------------------------------------------------
void ofApp::setKnobsSerial(){
    if (KnocsSerial.available() > 0) {
        if (KnocsSerial.available() == KNOB_NUM) {
            for (int index = 0; index < KNOB_NUM; index++) {
                knobs[index] = KnocsSerial.readByte();
            }
        }
    }
    KnocsSerial.flush();
}

//--------------------------------------------------------------
int ofApp::getBpm(){
    return knobs[0];//   bpm
}

//--------------------------------------------------------------
int ofApp::getOscillator(){
    return knobs[1];//   oscillator
}

//--------------------------------------------------------------
int ofApp::getDelay(){
    return knobs[2];//   delay
}

//--------------------------------------------------------------
int ofApp::getKick(){
    return knobs[3];//   kick
}

//--------------------------------------------------------------
int ofApp::getRim(){
    return knobs[4];//   rim
}

//--------------------------------------------------------------
int ofApp::getOctave(){
    return knobs[5];//   octave
}

//--------------------------------------------------------------
void ofApp::updateStepsSerialDebug(){
    for (int index = 0; index < STEP_SENDED_NUM; index++) {
        steps[index] = ofToInt(ofToString(stepsSlider[index].getParameter()));
    }
}

//--------------------------------------------------------------
void ofApp::updateKnobsSerialDebug(){
    for (int index = 0; index < KNOB_NUM; index++) {
        knobs[index] = ofToInt(ofToString(knobsSlider[index].getParameter()));
    }
}

//--------------------------------------------------------------
void ofApp::receiveOscBpmAndSendSerialLed(){
    
    // string msg = "";
    // msg += "FPS: " + ofToString( ofGetFrameRate() ) + "\n";
    
    int index = 0;
    while(OscReceiver.hasWaitingMessages()) {
        
        // Read Osc(max)
        ofxOscMessage m;
        OscReceiver.getNextMessage(&m);
        string value = m.getArgAsString(index);
        nowstep = m.getArgAsInt(index);
        
        if (m.getAddress() == "/count"){
            if (nowstep == 0) {
                roundCount++;
            }
            
            if (!IS_USE_GUI) {
                // Send serial(Arduion)
                // if you write byte from start, you cant send serial to arduino MEGA(only MEGA).
                if (millis() > 2000) {
                    unsigned char myByte =std::stoi(value);
                    bool byteWasWritten = StepsSerial.writeByte(myByte);
                    if (!byteWasWritten) printf("please reconnect arduino again. \n");
                }
            }
        }
        
        // ofDrawBitmapString( msg, 30, 30 );
    }
}

//--------------------------------------------------------------
void ofApp::sendOscSteps(){
    ofxOscMessage m;
    m.setAddress("/steps");
    for (int index = STEP_FIRST_NUM; index <= STEP_NUM; index++) {
        m.addIntArg(steps[index]);
        
        // debug
        // printf("%s \n", std::to_string(bytes[index]).c_str());
        // string stepStr = ofToString(index) + " : " + ofToString(bytes[index]);
        // ofDrawBitmapString(stepStr, 50, 30 * index + 10);
    }
    OscSender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::sendOscKonbs(){
    int index = 0;
    
    ofxOscMessage bpm;
    bpm.setAddress("/bpm");
    bpm.addIntArg(knobs[index++]);
    OscSender.sendMessage(bpm);

    ofxOscMessage oscillator;
    oscillator.setAddress("/oscillator");
    oscillator.addIntArg(knobs[index++]);
    OscSender.sendMessage(oscillator);

    ofxOscMessage delay;
    delay.setAddress("/delay");
    delay.addIntArg(knobs[index++]);
    OscSender.sendMessage(delay);

    ofxOscMessage kick;
    kick.setAddress("/kick");
    kick.addIntArg(knobs[index++]);
    OscSender.sendMessage(kick);
    
    ofxOscMessage rim;
    rim.setAddress("/rim");
    rim.addIntArg(knobs[index++]);
    OscSender.sendMessage(rim);
    
    ofxOscMessage octave;
    octave.setAddress("/octave");
    octave.addIntArg(knobs[index++]);
    OscSender.sendMessage(octave);
}

//--------------------------------------------------------------
void ofApp::drawSinCos(){
    ofPushMatrix();
    float circle = 200;
    float col;
    float freq = 0.000001 * getBpm() + 0.000001;
    float cont = 0;
    float r;
    
    translate(ofGetWidth()/2, ofGetHeight()/2);
    rotate(radians(drawSinCos_rot));
    
    //ellipseMode(RADIUS);
    for (float i=0; i<500; i+=5) {
        
        if (steps[getSameStepNum(i/5)] == 0) {
            continue;
        }
        
        circle = 30 * getReverceOctave() + 100 + 80*sin(millis()*freq*i);
        
        // col = map(circle, 150, 250, 255, 60);
        
        r = map(circle, 150, 250, 20*getDelay()/10+20, 8*getDelay());
        
        //fill(col, 255, 255);
        fill(getWoodenBallColor(steps[getSameStepNum(i/5)]));
        //noStroke();
        //stroke(col, 0, 74);
        drawOscillatorForm(circle*cos(i), circle*sin(i),r,r );
        drawSinCos_rot = drawSinCos_rot+0.00005;
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawCircleWave(){
    
    drawCircleWave_theta += getBpmSpeed();
    float x = drawCircleWave_theta;
    for (int i = 0; i < drawCircleWave_CIRCLE_COUNT; i++) {
        drawCircleWave_yvalues[i] = sin(x) * (drawCircleWave_h);
        x+=0.1 * getOctave() + 0.1;
    }
    
    //noStroke();
    for (int x = 0; x < drawCircleWave_CIRCLE_COUNT; x++) {
        
        float delaySize = getDelay() * 3;
        
        // only wooden balls are put.
        if (steps[getSameStepNum(x)] > 0) {
            ofColor color = getWoodenBallColor(steps[getSameStepNum(x)]);
            fill(color);
            drawOscillatorForm(x * (drawCircleWave_r + 4), windowHeight / 2 + drawCircleWave_yvalues[x], drawCircleWave_r * drawCircleWave_yvalues[x] / 80 + delaySize, drawCircleWave_r * drawCircleWave_yvalues[x] / 80 + delaySize);
            fill(color.invert());
            drawOscillatorForm(x * (drawCircleWave_r + 8), windowHeight  / 2 - drawCircleWave_yvalues[x], drawCircleWave_r * drawCircleWave_yvalues[x] / 80 + delaySize, drawCircleWave_r * drawCircleWave_yvalues[x] / 80 + delaySize);
        }
    }
}

//--------------------------------------------------------------
float ofApp::getBpmSpeed(){
    switch (getBpm()) {
        case 1:
            return 0.032;
            break;
        case 2:
            return 0.038;
            break;
        case 3:
            return 0.044;
            break;
        case 4:
            return 0.05;
            break;
        case 5:
            return 0.052;
            break;
        case 6:
            return 0.056;
            break;
        default:
            return 0.032;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::drawCircleTree(){
    ofPushMatrix();
    
    translate(windowWidth/2,0);
    for (int  y = drawCircleTree_radius+20; y <=windowHeight-20; y += drawCircleTree_diameter*1.3) {
        if (steps[getSameStepNum(y)] == 0) {
            continue;
        }
        float phi = y*90;
        float w=drawCircleTree_A*getReverceOctave()/3*sin(drawCircleTree_frequency*drawCircleTree_time + phi);
//        noStroke();
        drawCircleTree_col=map(w,-100,100,60,255);
        float b=map(y,100,600,0.1,2.3);
        float r =b*w/7;
        fill(getWoodenBallColor(steps[getSameStepNum(y)]));
        
        float delaySize = getDelay() * 4 + 2;
        
        drawOscillatorForm(w*b, y,r+delaySize,r+delaySize);
        drawOscillatorForm(-w*b, y,r+delaySize,r+delaySize);
    }
    drawCircleTree_time += getBpm() * 0.1 + getOctave() * 0.4 + 0.2;
    
    ofPopMatrix();
}

//--------------------------------------------------------------
int ofApp::getSameStepNum(int value){
    int checkNum = 16;
    if (value == 0) {
        return 16;
    } else if (value % checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    } else if (value % --checkNum == 0) {
        return checkNum;
    }
    return -1;
}

//--------------------------------------------------------------
void ofApp::background(int rgb){
    background(rgb, rgb, rgb);
}

//--------------------------------------------------------------
void ofApp::background(int r, int g, int b){
    ofBackground(r, g, b);
}

//--------------------------------------------------------------
void ofApp::translate(float x, float y){
    ofTranslate(x, y);
}

//--------------------------------------------------------------
void ofApp::rotate(float degrees){
    ofRotate(degrees);
}

//--------------------------------------------------------------
float ofApp::radians(float degree){
    return degree * (PI / 180);
}

//--------------------------------------------------------------
float ofApp::millis(){
    return ofGetElapsedTimeMillis() - startTimer;
}

//--------------------------------------------------------------
float ofApp::map(float v, float sx, float sn, float dx, float dn){
    return (v - sn) * (dx - dn) / (sx - sn) + dn;
}

//--------------------------------------------------------------
void ofApp::fill(ofColor color){
    ofSetColor(color);
    ofFill();
}

//--------------------------------------------------------------
void ofApp::fill(float r, float g, float b){
    ofSetColor(r,g,b);
    ofFill();
}

//--------------------------------------------------------------
void ofApp::strokeWeight(float lineWidth){
    ofSetLineWidth(lineWidth);
}

//--------------------------------------------------------------
float ofApp::sq(float value){
    return value * value;
}

//--------------------------------------------------------------
void ofApp::ellipse(float x, float y, float width, float height){
    ofSetCircleResolution(64);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::triangle(float x, float y, float width, float height){
    ofSetCircleResolution(3);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::rectangle(float x, float y, float width, float height){
    ofSetCircleResolution(4);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::pentagon(float x, float y, float width, float height){
    ofSetCircleResolution(5);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::hexagon(float x, float y, float width, float height){
    ofSetCircleResolution(6);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::heptagon(float x, float y, float width, float height){
    ofSetCircleResolution(7);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::octagon(float x, float y, float width, float height){
    ofSetCircleResolution(8);
    ofEllipse(x,y,width,height);
}

//--------------------------------------------------------------
void ofApp::drawOscillatorForm(float x, float y, float width, float height){
    switch (getOscillator()) {
        case 1:
            ofNoFill();
            ellipse(x, y, width, height);
            break;
        case 2:
            ellipse(x, y, width, height);
            break;
        case 3:
            hexagon(x, y, width, height);
            break;
        case 4:
            pentagon(x, y, width, height);
            break;
        case 5:
            rectangle(x, y, width, height);
            break;
        case 6:
            triangle(x, y, width, height);
            break;
        default:
            break;
    }
}

int ofApp::getReverceOctave(){
    switch (getOctave()) {
        case 1:
            return 6;
            break;
        case 2:
            return 5;
            break;
        case 3:
            return 4;
            break;
        case 4:
            return 3;
            break;
        case 5:
            return 2;
            break;
        case 6:
            return 1;
            break;
        default:
            return 1;
            break;
    }
}

//--------------------------------------------------------------
ofColor ofApp::getWoodenBallColor(int index){
    ofColor color;
    switch (index) {
        case 0:
            color.set(255, 255, 255);
            break;
        case 1:
            color.set(225, 84, 78);
            break;
        case 2:
            color.set(235, 133, 51);
            break;
        case 3:
            color.set(236, 196, 61);
            break;
        case 4:
            color.set(170, 225, 86);
            break;
        case 5:
            color.set(122, 215, 120);
            break;
        case 6:
            color.set(133, 198, 216);
            break;
        case 7:
            color.set(112, 154, 214);
            break;
        case 8:
            color.set(193, 123, 198);
            break;
        default:
            color.set(255, 255, 255);
            break;
    }
    return color;
}
