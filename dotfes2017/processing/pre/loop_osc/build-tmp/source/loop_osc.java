import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import netP5.*; 
import oscP5.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class loop_osc extends PApplet {






Serial myPort;

final int BPM = 60;

final int STEP_NUM = 8;
float fillColor;
float diameter;

OscP5 oscP5;
NetAddress maxLocation;

//final String serialPortName = "/dev/cu.usbmodem1461";
final String serialPortName = Serial.list()[3];

//final String toMaxIPAddr = "192.168.43.19";
final String toMaxIPAddr = "127.0.0.1";

int gDataCnt = 0;
int gStepCnt = 0;
int gSteps[] = new int[STEP_NUM];
int gBpm = 0;
int gOctave = 0;
int gDelay = 0;
int gTempo = 0;
int gOScillator = 0;

// boolean isStart = false;

public void setup(){
  //size(640,480);

  //Serial Port Open
  myPort = new Serial(this, serialPortName, 115200);

  /* \u53d7\u4fe1\u7528\u306e\u5909\u6570\u3002\u53f3\u306e\u6570\u5b57\u306f\u30dd\u30fc\u30c8\u756a\u53f7\u3002\u9001\u4fe1\u5074\u306e\u30dd\u30fc\u30c8\u756a\u53f7\u3068\u3042\u308f\u305b\u308b\u3002 */
  oscP5 = new OscP5(this,7100);

  //\u9001\u4fe1\u7528\u30aa\u30d6\u30b8\u30a7\u30af\u30c8\u3002\u5de6\u5074\u306e\u6570\u5b57\u304c\u76f8\u624b\u306eIP\u30a2\u30c9\u30ec\u30b9\u3001\u53f3\u5074\u304c\u76f8\u624b\u306e\u30dd\u30fc\u30c8\u756a\u53f7\u3002
  maxLocation = new NetAddress(toMaxIPAddr, 7400);

  //\u30c7\u30fc\u30bf\u3092\u9001\u308b\u5148\u306e\u95a2\u6570\u3092\u767b\u9332\u3059\u308b\u3002\u3053\u3053\u3067\u306f\u3001getData\u306f\u76f8\u624b\u5148\u306e\u95a2\u6570\u3002
  //\u300c/count\u300d\u306f\u9001\u4fe1\u5074\u3068\u53d7\u4fe1\u5074\u3067\u540c\u3058\u3067\u3042\u308b\u5fc5\u8981\u304c\u3042\u308b\u6697\u53f7\u306e\u3088\u3046\u306a\u3082\u306e
  oscP5.plug(this, "syncStart","/start");
  oscP5.plug(this, "forceSync","/sync");
  oscP5.plug(this, "syncStop", "/stop");
  oscP5.plug(this, "playStep", "/count");
}

public void draw(){
  //background(0);
  //fill(fillColor);
  //ellipse(width/2,height/2,diameter,diameter);

  //testCode();
}

//OSC Receive Event (Counter)
public void syncStart(){
     println("start");
    // isStart = true;
    myPort.write(1);
}

public void forceSync(){
     println("sync");
    myPort.write(2);
}

public void syncStop(){
     println("stop");
    myPort.write(3);
}

public void playStep(int count) {
  OscMessage stepMsg = new OscMessage("/steps");
  
  if (gTempo == 1) {
    if (count % 2 == 0) {
      stepMsg.add(count/2);   
      oscP5.send(stepMsg, maxLocation); //\u9001\u4fe1
    }
  }
  else if (gTempo == 2) {
    if (count >= 8) {
     count = count - 8; 
    }
    stepMsg.add(count);
    oscP5.send(stepMsg, maxLocation); //\u9001\u4fe1
  }
}

//Serial Receive Event
public void serialEvent(Serial myPort) {
  int resvValue = 0;

  resvValue = myPort.read();

  if (gDataCnt == 0) {
    gStepCnt = resvValue;
  }
  else if (1 <= gDataCnt && gDataCnt <= 8) {
    gSteps[gDataCnt - 1] = resvValue;
  }
  else if (gDataCnt == 9) {
    //0/1\u3067MAX\u306b\u6e21\u3059
    gOctave = resvValue - 1;
  }
  else if (gDataCnt == 10) {
    //0/1\u3067Max\u306b\u6e21\u3059
    gDelay = resvValue - 1;
  }
  else if (gDataCnt == 11) {
    //0/1\u3067Max\u306b\u6e21\u3059
    gTempo = resvValue;
  }
  else if (gDataCnt == 12) {
    //0/1\u3067Max\u306b\u6e21\u3059
    gOScillator = resvValue - 1;
  }

  try {
    //Send Steps
    if (gDataCnt == 0) {
      //println("count: " + gStepCnt);
      OscMessage countMsg = new OscMessage("/counter");
      countMsg.add(gStepCnt);
      oscP5.send(countMsg, maxLocation); //\u9001\u4fe1
    }
    //Send Steps
    else if (gDataCnt == 8) {
      //println("steps: "+gSteps);
      OscMessage stepMsg = new OscMessage("/steps");
      stepMsg.add(gSteps);
      oscP5.send(stepMsg, maxLocation); //\u9001\u4fe1
    }
    //Send Octave
    else if (gDataCnt == 9) {
      // println("octave: " + gOctave);
      OscMessage octaveMsg = new OscMessage("/octave");
      octaveMsg.add(gOctave);
      oscP5.send(octaveMsg, maxLocation); //\u9001\u4fe1
    }
    //Send Delay
    else if (gDataCnt == 10) {
      // println("delay: " + gDelay);
      OscMessage delayMsg = new OscMessage("/delay");
      delayMsg.add(gDelay);
      oscP5.send(delayMsg, maxLocation); //\u9001\u4fe1
    }
    //Send Delay
    else if (gDataCnt == 11) {
      // println("tempo: " + gTempo);
      //OscMessage delayMsg = new OscMessage("/delay");
      //delayMsg.add(gDelay);
      //oscP5.send(delayMsg, maxLocation); //\u9001\u4fe1
    }
    else if (gDataCnt == 12) {
      // println("OScillator: " + gOScillator);
      //OscMessage oscillatorMsg = new OscMessage("/oscillo");
      //oscillatorMsg.add(gOScillator);
      //oscP5.send(oscillatorMsg, maxLocation);
    }
  }
  catch (RuntimeException e) {
    println("Error:OSC Message Send..");
  }

  gDataCnt++;
  if (12 == gDataCnt) {
    gDataCnt = 0;
  }
}

//void testCode() {

//  if (true != isStart) {
//    return;
//  }
//  OscMessage countMsg = new OscMessage("/counter");
//  countMsg.add(gStepCnt);
//  oscP5.send(countMsg, maxLocation); //\u9001\u4fe1

//  delay((60 / BPM) * (4 / 4) * 1000);
//}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "loop_osc" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
