
import netP5.*;
import oscP5.*;
import processing.serial.*;

Serial myPort;

final int BPM = 60;

final int STEP_NUM = 8;


OscP5 oscP5;
NetAddress maxLocation;

// ここを自分のシリアルポートに変更する
final String serialPortName = "/dev/cu.usbmodem1451";


//final String toMaxIPAddr = "192.168.43.19";
final String toMaxIPAddr = "127.0.0.1";

int gDataCnt = 0;
int gStepCnt = 0;
int gSteps[] = new int[STEP_NUM];
int gBpm = 0;
int gOctave = 0;
int gDelay = 0;
int gOScillator = 0;
int gTempo = 0;

// boolean isStart = false;


void setup(){
  //size(640,480);

  //Serial Port Open
  myPort = new Serial(this, serialPortName, 115200);

  /* 受信用の変数。右の数字はポート番号。送信側のポート番号とあわせる。 */
  oscP5 = new OscP5(this,7100);

  //送信用オブジェクト。左側の数字が相手のIPアドレス、右側が相手のポート番号。
  maxLocation = new NetAddress(toMaxIPAddr, 7400);

  //データを送る先の関数を登録する。ここでは、getDataは相手先の関数。
  //「/count」は送信側と受信側で同じである必要がある暗号のようなもの
  // oscP5.plug(this, "syncStart","/start");
  // oscP5.plug(this, "forceSync","/sync");
  // oscP5.plug(this, "syncStop", "/stop");
  oscP5.plug(this, "playStep", "/count");
}

void draw(){
  //background(0);
  //testCode();
}

//OSC Receive Event (Counter)
// public void syncStart(){
//      println("start");
//     // isStart = true;
//     myPort.write(1);
// }

// public void forceSync(){
//      println("sync");
//     myPort.write(2);
// }

// public void syncStop(){
//      println("stop");
//     myPort.write(3);
// }


public void playStep(int aCount) {

  //println("aCount: "+aCount);
  myPort.write(aCount);
  // OscMessage stepMsg = new OscMessage("/steps");

//   if (gTempo == 1) {
//     if (aCount % 2 == 0) {
//       stepMsg.add(aCount/2);
//       oscP5.send(stepMsg, maxLocation); //送信
//     }
//   }
//   else if (gTempo == 2) {
//     if (aCount >= 8) {
//      aCount = aCount - 8;
//     }
//     stepMsg.add(aCount);
//     oscP5.send(stepMsg, maxLocation); //送信
//   }
}

//Serial Receive Event
void serialEvent(Serial myPort) {
    int resvValue = 0;
    resvValue = myPort.read();



  if (gDataCnt == 0) {
    // gStepCnt = resvValue;
  }
  else if (1 <= gDataCnt && gDataCnt <= 8) {
    gSteps[gDataCnt - 1] = resvValue;
  }
  else if (gDataCnt == 9) {
    //BPM
    //0/1でMAXに渡す
    // gBpm = resvValue;
  }
  else if (gDataCnt == 10) {
    //octave
    //0/1でMaxに渡す
    // gOctave = resvValue - 1;
  }
  else if (gDataCnt == 11) {
    //0/1でMaxに渡す
    // gDelay = resvValue - 1;
  }
  else if (gDataCnt == 12) {
    //oscillator
    //0/1でMaxに渡す
    // gOScillator = resvValue - 1;
  }

  try {
    //Send Steps
    if (gDataCnt == 0) {
      // println("count: " + gStepCnt);
      // OscMessage countMsg = new OscMessage("/counter");
      // countMsg.add(gStepCnt);
      // oscP5.send(countMsg, maxLocation); //送信
    }
    //Send Steps
    if (gDataCnt == 8) {
      // println("step: " + gSteps);
      OscMessage stepMsg = new OscMessage("/steps");
      stepMsg.add(gSteps);
      oscP5.send(stepMsg, maxLocation); //送信
      println("stepMsg: " + stepMsg);
    }
    else if (gDataCnt == 9) {
      // println("BPM: " +gBpm);
      // OscMessage bpmMsg = new OscMessage("/bpm");
      // bpmMsg.add(gBpm);
      // oscP5.send(bpmMsg, maxLocation);
    }
    //Send Octave
    else if (gDataCnt == 10) {
      // println("Octave: " + gOctave);
      // OscMessage octaveMsg = new OscMessage("/octave");
      // octaveMsg.add(gOctave);
      // oscP5.send(octaveMsg, maxLocation); //送信
    }
    //Send Delay
    else if (gDataCnt == 11) {
      // println("Delay" + gDelay);
      // OscMessage delayMsg = new OscMessage("/delay");
      // delayMsg.add(gDelay);
      // oscP5.send(delayMsg, maxLocation); //送信
    }
    else if (gDataCnt == 12) {
      // println("OScillator" + gOScillator);
      // OscMessage oscillatorMsg = new OscMessage("/oscillo");
      // oscillatorMsg.add(gOScillator);
      // oscP5.send(oscillatorMsg, maxLocation);
    }
  }
  catch (RuntimeException e) {
    println("Error:OSC Message Send..");
  }

  gDataCnt++;

  if (13 == gDataCnt) {
    gDataCnt = 0;
  }

  println(gSteps);
  println("resvValue: "+resvValue);
}

//void testCode() {

//  if (true != isStart) {
//    return;
//  }
//  OscMessage countMsg = new OscMessage("/counter");
//  countMsg.add(gStepCnt);
//  oscP5.send(countMsg, maxLocation); //送信

//  delay((60 / BPM) * (4 / 4) * 1000);
//}
